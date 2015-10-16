#include "ps/base.h"
#include "ps/internal/customer.h"
namespace ps {

/**
 * \brief the structure for a list of key-value pairs
 *
 * The keys must be unique and sorted in an increasing order.  The length of a
 * value can be more than one. If \a lens is empty, then the length
 * of a value is determined by `k=vals.size()/keys.size()`.  The \a i-th KV pair
 * is then
 *
 * \verbatim {keys[i], (vals[i*k], ..., vals[(i+1)*k-1])} \endverbatim
 *
 * If \a lens is given, then `lens[i]` is the length of the \a i-th
 * value. Let
 *
 * \verbatim n = lens[0] + .. + lens[i-1]  \endverbatim
 *
 * then the \a i-th KV pair is presented as
 *
 * \verbatim {keys[i], (vals[n], ..., vals[lens[i]+n-1])} \endverbatim
 */
template <typename Val>
struct KVPairs {
  // /** \brief empty constructor */
  // KVPairs() {}
  /** \brief the list of keys */
  SArray<Key> keys;
  /** \brief the according values */
  SArray<Val> vals;
  /** \brief the according value lengths (could be empty) */
  SArray<int> lens;
};

/**
 * \brief A worker node that can \ref Push (\ref Pull) key-value pairs to (from) server
 * nodes
 *
 * \tparam Val the type of value, which should be primitive types such as
 * int32_t and float
 */
template<typename Val>
class KVWorker {
 public:
  /**
   * \brief constructor
   *
   * \param app_id the app id, should match with \ref KVServer's id
   */
  explicit KVWorker(int app_id) :
      obj_(app_id, [this](const Message& recv){ RecvHandle(recv); }) {
    // TODO
    // slicer_ = ...
  }

  /** \brief deconstructor */
  ~KVWorker() { }

  /**
   * \brief callback function for \ref Push and \ref Pull
   *
   * It is called by the data receiving thread of this instance when the push or
   * pull is actually finished. Namely the kv pairs have already written into
   * servers' data structure or the kv pairs have already pulled back.
   */
  using Callback = std::function<void()>;

  /**
   * \brief Pushes a list of key-value pairs to all server nodes.
   *
   * This function pushes a KV list specified by \a keys and \a vals to all
   * server nodes.
   *
   * Sample usage: the following codes push two KV pairs `{1, (1.1, 1.2)}` and `{3,
   * (3.1,3.2)}` to server nodes, where the value is a length-2 float vector
   * \code
   *   KVWorker<float> w;
   *   std::vector<Key> keys = {1, 3};
   *   std::vector<float> vals = {1.1, 1.2, 3.1, 3.2};
   *   w.Push(keys, vals);
   * \endcode
   *
   * If \a lens is given, then the value can be various length. See
   * \ref KVPairs for more information.
   *
   * The KV list is partitioned and sent based on the key range each server
   * maintaining. This function returns without waiting the data are sent
   * actually. Instead, use either \ref Wait or the callback to know when
   * finished. This function is thread-safe.
   *
   * @param keys a list of keys, must be unique and sorted in increasing order
   * @param vals the according values
   * @param lens optional, lens[i] stores the value length of the \a
   * i-th KV pair
   * @param cmd an optional command sent to the servers
   * @param cb the callback which is called when the push is finished.
   * @return the timestamp of this request
   */
  int Push(const std::vector<Key>& keys,
           const std::vector<Val>& vals,
           const std::vector<int>& lens = {},
           int cmd = 0,
           const Callback& cb = nullptr) {
    return ZPush(
        SArray<Key>(keys), SArray<Val>(vals), SArray<int>(lens), cmd, cb);
  }

  /**
   * \brief Pulls the values associated with the keys from the server nodes
   *
   * This function pulls the values of the keys specified in \a keys from the
   * server nodes. The format is same to \ref KVPairs
   *
   * Sample usage: the following codes pull the values of keys \a 1 and \a 3
   * from the server nodes.
   * \code
   *   KVWorker<float> w;
   *   std::vector<Key> keys = {1, 3};
   *   std::vector<float> vals;
   *   ps.Pull(keys, &vals);
   * \endcode
   *
   * It's a non-blocking call. The actual pulling is finished,
   * namely \a vals (and \a lens) is filled with pulled values, only
   * if \ref Wait returns or the callback is called.
   *
   * @param keys a list of keys, must be unique and sorted in increasing order
   * @param vals the buffer for the pulled values. It can be 0 size.
   * @param lens optional buffer for the value length. If set, it can be 0 size.
   * @param cmd an optional command sent to the servers
   * @param cb the callback which is called when the pull is finished.
   * @return the timestamp of this request
   */
  int Pull(const std::vector<Key>& keys,
           std::vector<Val>* vals,
           std::vector<int>* lens = nullptr,
           int cmd = 0,
           const Callback& cb = nullptr) {
    return Pull_(SArray<Key>(keys), vals, lens, cmd, cb);
  }

  /**
   * \brief Waits until a push or pull has been finished
   *
   * Sample usage:
   * \code
   *   int ts = w.Pull(keys, &vals);
   *   Wait(ts);
   *   // now vals is ready for use
   * \endcode
   *
   * \param timestamp the timestamp returned by the push or pull
   */
  void Wait(int timestamp) { obj_.WaitRequest(timestamp); }

  /**
   * \brief zero-copy Push
   *
   * This function is similar to \ref Push except that all data
   * will not be copied into system for better performance. It is the caller's
   * responsibility to keep the content to be not changed before actually
   * finished.
   */
  int ZPush(const SArray<Key>& keys,
            const SArray<Val>& vals,
            const SArray<int>& lens = {},
            int cmd = 0,
            const Callback& cb = nullptr) {
    int ts = obj_.NewRequest(kServerGroup);
    AddCallback(ts, cb);
    KVPairs<Val> kvs;
    kvs.keys = keys;
    kvs.vals = vals;
    kvs.lens = lens;
    Send(ts, true, cmd, kvs);
  }

  /**
   * \brief zero-copy Pull
   *
   * This function is similar to \ref Pull except that all data
   * will not be copied into system for better performance. It is the caller's
   * responsibility to keep the content to be not changed before actually
   * finished.
   */
  int ZPull(const SArray<Key>& keys,
            SArray<Val>* vals,
            SArray<int>* lens = nullptr,
            int cmd = 0,
            const Callback& cb = nullptr) {
    return Pull_(keys, vals, lens, cmd, cb);
  }

  using SlicedKVs = std::vector<std::pair<bool, KVPairs<Val>>>;
  /**
   * \brief a slicer partitions a key-value list according to the key ranges
   * \param send the kv list for partitioning
   * \param ranges the key ranges, ranges[i] is the key range of server i
   * \param sliced the sliced lists. slices[i] should only contains keys in
   * ranges[i] and the according values
   */
  using Slicer = std::function<void(
      const KVPairs<Val>& send, const std::vector<Range>& ranges,
      SlicedKVs* sliced)>;

  /**
   * \brief set a user-defined slicer
   */
  void set_slicer(const Slicer& slicer) {
    CHECK(slicer); slicer_ = slicer;
  }

 private:
  /**
   * \brief internal pull, C/D can be either SArray or std::vector
   */
  template <typename C, typename D>
  int Pull_(const SArray<Key>& keys, C* vals, D* lens,
            int cmd, const Callback& cb);

  /**
   * \brief add a callback for a request. threadsafe.
   * @param cb callback
   * @param timestamp the timestamp of the request
   */
  void AddCallback(int timestamp, const Callback& cb) {
    if (!cb) return;
    std::lock_guard<std::mutex> lk(mu_);
    callbacks_[timestamp] = cb;
  }

  /**
   * \brief send the kv list to all servers
   * @param timestamp the timestamp of the request
   * @param push whether or not it is a push request
   * @param cmd command
   */
  void Send(int timestamp, bool push, int cmd, const KVPairs<Val>& kvs);

  /** \brief internal receive handle */
  void RecvHandle(const Message& msg);

  // static void DefaultSlicer(const KVPairs& send,
  //                           const std::vector<Range>& ranges,
  //                           SlicedKVs* sliced) {
  // }

  /** \brief data buffer for received kvs for each timestamp */
  std::unordered_map<int, std::vector<KVPairs<Val>>> recv_kvs_;
  /** \brief callbacks for each timestamp */
  std::unordered_map<int, Callback> callbacks_;
  /** \brief lock */
  std::mutex mu_;
  /** \brief kv list slicer */
  Slicer slicer_;
  /** \brief ps internal object */
  Customer obj_;
};


/**
 * \brief A server node for maintaining key-value pairs
 *
 */
template <typename Val>
class KVServer {
 public:
  /** \brief meta information about the request */
  struct ReqMeta {
    /** \brief the int cmd */
    int cmd;
    /** \brief whether or not this is a push request */
    bool push;
    /** \brief sender's node id */
    int sender;
    /** \brief the associated timestamp */
    int timestamp;
  };

  /**
   * \brief the handle to process a push/pull request from a worker
   * \param recved one request received from one worker
   * \param server this pointer
   */
  using ReqHandle = std::function<void(const ReqMeta& req_meta,
                                       const KVPairs<Val>& req_data,
                                       KVServer* server)>;
  /**
   * \brief constructor
   * \param app_id the app id, should match with \ref KVWorker's id
   * \param req_handle the handle for processing a request
   */
  KVServer(int app_id, const ReqHandle& req_handle) :
      req_handle_(req_handle),
      obj_(app_id, [this](const Message& msg){ RecvHandle(msg); }) {
    CHECK(req_handle_) << "invalid request handle";
  }

  /**
   * \brief response to the push/pull request
   * \param req the request received from the worker
   * \param res the response that will send back to the worker
   */
  void Response(const ReqMeta& req_meta, const KVPairs<Val>& res = KVPairs<Val>());

 private:
  /** \brief internal receive handle */
  void RecvHandle(const Message& msg);

  /** \brief request handle */
  ReqHandle req_handle_;
  /** \brief ps internal object */
  Customer obj_;
};


///////////////////////////////////////////////////////////////////////////////


template <typename Val>
template <typename C, typename D>
int KVWorker<Val>::Pull_(
    const SArray<Key>& keys, C* vals, D* lens, int cmd, const Callback& cb) {
  int ts = obj_.NewRequest(kServerGroup);
  AddCallback(ts, [this, ts, keys, vals, lens, cb]() {
      mu_.lock();
      auto& kvs = recv_kvs_[ts];
      mu_.unlock();

      // do check
      size_t total_key = 0, total_val = 0;
      for (const auto& s : kvs) {
        Range range = FindRange(keys, s.keys.front(), s.keys.back()+1);
        CHECK_EQ(range.size(), s.keys.size())
            << "unmatched keys size from one server";
        if (lens) CHECK_EQ(s.lens.size(), s.keys.size());
        total_key += s.keys.size();
        total_val += s.vals.size();
      }
      CHECK_EQ(total_key, keys.size()) << "lost some servers?";

      // fill vals and lens
      std::sort(kvs.begin(), kvs.end(), [](
          const KVPairs<Val>& a, const KVPairs<Val>& b) {
                  return a.keys.front() < b.keys.front();
        });
      CHECK_NOTNULL(vals)->resize(total_val);
      Val* p_vals = vals->data();
      int *p_lens = nullptr;
      if (lens) {
        lens->resize(keys.size());
        p_lens = lens->data();
      }
      for (const auto& s : kvs) {
        memcpy(p_vals, s.vals.data(), s.vals.size() * sizeof(Val));
        p_vals += s.vals.size();
        if (p_lens) {
          memcpy(p_lens, s.lens.data(), s.lens.size() * sizeof(int));
          p_lens += s.lens.size();
        }
      }
      // keys.clear(); // make sure it is delete

      mu_.lock();
      recv_kvs_.erase(ts);
      mu_.unlock();
      cb();
    });

  KVPairs<Val> kvs; kvs.keys = keys;
  Send(ts, false, cmd, kvs);
  return ts;
}


template <typename Val>
void KVWorker<Val>::Send(int timestamp, bool push, int cmd, const KVPairs<Val>& kvs) {
  SlicedKVs sliced;
  slicer_(kvs, Postoffice::Get()->GetServerKeyRanges(), &sliced);
  int skipped = 0;
  for (size_t i = 0; i < sliced.size(); ++i) {
    const auto& s = sliced[i];
    if (!s.first) { ++ skipped; continue; }
    Message msg;
    msg.meta.set_customer_id(obj_.id());
    msg.meta.set_request(true);
    msg.meta.set_push(push);
    msg.meta.set_head(cmd);
    msg.meta.set_timestamp(timestamp);
    if (s.keys.size()) {
      msg.AddData(s.keys);
      msg.AddData(s.vals);
      if (s.lens.size()) {
        msg.AddData(s.lens);
      }
    }
    msg.recver = i;
    Postoffice::Get()->van()->Send(msg);
  }
  obj_.AddResponse(timestamp, skipped);
}


template <typename Val>
void KVWorker<Val>::RecvHandle(const Message& msg) {
  // store the data for pulling
  int ts = msg.meta.timestamp();
  if (!msg.meta.push() && msg.data.size()) {
    CHECK_GT(msg.data.size(), (size_t)2);
    KVPairs<Val> kvs;
    kvs.keys = msg.data[0];
    kvs.vals = msg.data[1];
    if (msg.data.size() > (size_t)2) {
      kvs.lens = msg.data[2];
    }
    mu_.lock();
    auto& kv = recv_kvs_[ts].push_back(kvs);
    mu_.unlock();
  }

  // finished, run callbacks
  if (obj_.NumResponse(ts) == NumServers() - 1)  {
    mu_.lock();
    const auto& cb = callbacks_[ts];
    mu_.unlock();
    cb();

    mu_.lock();
    callbacks_.erase(ts);
    mu_.unlock();

  }
}

}  // namespace ps
