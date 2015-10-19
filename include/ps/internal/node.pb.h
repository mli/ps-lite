// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ps/internal/node.proto

#ifndef PROTOBUF_ps_2finternal_2fnode_2eproto__INCLUDED
#define PROTOBUF_ps_2finternal_2fnode_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace ps {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_ps_2finternal_2fnode_2eproto();
void protobuf_AssignDesc_ps_2finternal_2fnode_2eproto();
void protobuf_ShutdownFile_ps_2finternal_2fnode_2eproto();

class Node;

enum Node_Role {
  Node_Role_SERVER = 0,
  Node_Role_WORKER = 1,
  Node_Role_SCHEDULER = 3
};
bool Node_Role_IsValid(int value);
const Node_Role Node_Role_Role_MIN = Node_Role_SERVER;
const Node_Role Node_Role_Role_MAX = Node_Role_SCHEDULER;
const int Node_Role_Role_ARRAYSIZE = Node_Role_Role_MAX + 1;

const ::google::protobuf::EnumDescriptor* Node_Role_descriptor();
inline const ::std::string& Node_Role_Name(Node_Role value) {
  return ::google::protobuf::internal::NameOfEnum(
    Node_Role_descriptor(), value);
}
inline bool Node_Role_Parse(
    const ::std::string& name, Node_Role* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Node_Role>(
    Node_Role_descriptor(), name, value);
}
// ===================================================================

class Node : public ::google::protobuf::Message {
 public:
  Node();
  virtual ~Node();

  Node(const Node& from);

  inline Node& operator=(const Node& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const Node& default_instance();

  void Swap(Node* other);

  // implements Message ----------------------------------------------

  Node* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const Node& from);
  void MergeFrom(const Node& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  typedef Node_Role Role;
  static const Role SERVER = Node_Role_SERVER;
  static const Role WORKER = Node_Role_WORKER;
  static const Role SCHEDULER = Node_Role_SCHEDULER;
  static inline bool Role_IsValid(int value) {
    return Node_Role_IsValid(value);
  }
  static const Role Role_MIN =
    Node_Role_Role_MIN;
  static const Role Role_MAX =
    Node_Role_Role_MAX;
  static const int Role_ARRAYSIZE =
    Node_Role_Role_ARRAYSIZE;
  static inline const ::google::protobuf::EnumDescriptor*
  Role_descriptor() {
    return Node_Role_descriptor();
  }
  static inline const ::std::string& Role_Name(Role value) {
    return Node_Role_Name(value);
  }
  static inline bool Role_Parse(const ::std::string& name,
      Role* value) {
    return Node_Role_Parse(name, value);
  }

  // accessors -------------------------------------------------------

  // required .ps.Node.Role role = 1;
  inline bool has_role() const;
  inline void clear_role();
  static const int kRoleFieldNumber = 1;
  inline ::ps::Node_Role role() const;
  inline void set_role(::ps::Node_Role value);

  // optional int32 id = 2;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 2;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional string hostname = 3;
  inline bool has_hostname() const;
  inline void clear_hostname();
  static const int kHostnameFieldNumber = 3;
  inline const ::std::string& hostname() const;
  inline void set_hostname(const ::std::string& value);
  inline void set_hostname(const char* value);
  inline void set_hostname(const char* value, size_t size);
  inline ::std::string* mutable_hostname();
  inline ::std::string* release_hostname();
  inline void set_allocated_hostname(::std::string* hostname);

  // optional int32 port = 4;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 4;
  inline ::google::protobuf::int32 port() const;
  inline void set_port(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:ps.Node)
 private:
  inline void set_has_role();
  inline void clear_has_role();
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_hostname();
  inline void clear_has_hostname();
  inline void set_has_port();
  inline void clear_has_port();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  int role_;
  ::google::protobuf::int32 id_;
  ::std::string* hostname_;
  ::google::protobuf::int32 port_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_ps_2finternal_2fnode_2eproto();
  friend void protobuf_AssignDesc_ps_2finternal_2fnode_2eproto();
  friend void protobuf_ShutdownFile_ps_2finternal_2fnode_2eproto();

  void InitAsDefaultInstance();
  static Node* default_instance_;
};
// ===================================================================


// ===================================================================

// Node

// required .ps.Node.Role role = 1;
inline bool Node::has_role() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void Node::set_has_role() {
  _has_bits_[0] |= 0x00000001u;
}
inline void Node::clear_has_role() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void Node::clear_role() {
  role_ = 0;
  clear_has_role();
}
inline ::ps::Node_Role Node::role() const {
  return static_cast< ::ps::Node_Role >(role_);
}
inline void Node::set_role(::ps::Node_Role value) {
  assert(::ps::Node_Role_IsValid(value));
  set_has_role();
  role_ = value;
}

// optional int32 id = 2;
inline bool Node::has_id() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void Node::set_has_id() {
  _has_bits_[0] |= 0x00000002u;
}
inline void Node::clear_has_id() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void Node::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 Node::id() const {
  return id_;
}
inline void Node::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional string hostname = 3;
inline bool Node::has_hostname() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void Node::set_has_hostname() {
  _has_bits_[0] |= 0x00000004u;
}
inline void Node::clear_has_hostname() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void Node::clear_hostname() {
  if (hostname_ != &::google::protobuf::internal::kEmptyString) {
    hostname_->clear();
  }
  clear_has_hostname();
}
inline const ::std::string& Node::hostname() const {
  return *hostname_;
}
inline void Node::set_hostname(const ::std::string& value) {
  set_has_hostname();
  if (hostname_ == &::google::protobuf::internal::kEmptyString) {
    hostname_ = new ::std::string;
  }
  hostname_->assign(value);
}
inline void Node::set_hostname(const char* value) {
  set_has_hostname();
  if (hostname_ == &::google::protobuf::internal::kEmptyString) {
    hostname_ = new ::std::string;
  }
  hostname_->assign(value);
}
inline void Node::set_hostname(const char* value, size_t size) {
  set_has_hostname();
  if (hostname_ == &::google::protobuf::internal::kEmptyString) {
    hostname_ = new ::std::string;
  }
  hostname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* Node::mutable_hostname() {
  set_has_hostname();
  if (hostname_ == &::google::protobuf::internal::kEmptyString) {
    hostname_ = new ::std::string;
  }
  return hostname_;
}
inline ::std::string* Node::release_hostname() {
  clear_has_hostname();
  if (hostname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = hostname_;
    hostname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void Node::set_allocated_hostname(::std::string* hostname) {
  if (hostname_ != &::google::protobuf::internal::kEmptyString) {
    delete hostname_;
  }
  if (hostname) {
    set_has_hostname();
    hostname_ = hostname;
  } else {
    clear_has_hostname();
    hostname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 port = 4;
inline bool Node::has_port() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void Node::set_has_port() {
  _has_bits_[0] |= 0x00000008u;
}
inline void Node::clear_has_port() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void Node::clear_port() {
  port_ = 0;
  clear_has_port();
}
inline ::google::protobuf::int32 Node::port() const {
  return port_;
}
inline void Node::set_port(::google::protobuf::int32 value) {
  set_has_port();
  port_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ps

#ifndef SWIG
namespace google {
namespace protobuf {

template <>
inline const EnumDescriptor* GetEnumDescriptor< ::ps::Node_Role>() {
  return ::ps::Node_Role_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ps_2finternal_2fnode_2eproto__INCLUDED
