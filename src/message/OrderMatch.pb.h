// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: OrderMatch.proto

#ifndef PROTOBUF_INCLUDED_OrderMatch_2eproto
#define PROTOBUF_INCLUDED_OrderMatch_2eproto

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3006001
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#define PROTOBUF_INTERNAL_EXPORT_protobuf_OrderMatch_2eproto 

namespace protobuf_OrderMatch_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[1];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void AddDescriptors();
}  // namespace protobuf_OrderMatch_2eproto
namespace message {
class OrderMatch;
class OrderMatchDefaultTypeInternal;
extern OrderMatchDefaultTypeInternal _OrderMatch_default_instance_;
}  // namespace message
namespace google {
namespace protobuf {
template<> ::message::OrderMatch* Arena::CreateMaybeMessage<::message::OrderMatch>(Arena*);
}  // namespace protobuf
}  // namespace google
namespace message {

// ===================================================================

class OrderMatch : public ::google::protobuf::Message /* @@protoc_insertion_point(class_definition:message.OrderMatch) */ {
 public:
  OrderMatch();
  virtual ~OrderMatch();

  OrderMatch(const OrderMatch& from);

  inline OrderMatch& operator=(const OrderMatch& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  OrderMatch(OrderMatch&& from) noexcept
    : OrderMatch() {
    *this = ::std::move(from);
  }

  inline OrderMatch& operator=(OrderMatch&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  static const ::google::protobuf::Descriptor* descriptor();
  static const OrderMatch& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const OrderMatch* internal_default_instance() {
    return reinterpret_cast<const OrderMatch*>(
               &_OrderMatch_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  void Swap(OrderMatch* other);
  friend void swap(OrderMatch& a, OrderMatch& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline OrderMatch* New() const final {
    return CreateMaybeMessage<OrderMatch>(NULL);
  }

  OrderMatch* New(::google::protobuf::Arena* arena) const final {
    return CreateMaybeMessage<OrderMatch>(arena);
  }
  void CopyFrom(const ::google::protobuf::Message& from) final;
  void MergeFrom(const ::google::protobuf::Message& from) final;
  void CopyFrom(const OrderMatch& from);
  void MergeFrom(const OrderMatch& from);
  void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) final;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const final;
  ::google::protobuf::uint8* InternalSerializeWithCachedSizesToArray(
      bool deterministic, ::google::protobuf::uint8* target) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(OrderMatch* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::google::protobuf::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // string symbol = 1;
  void clear_symbol();
  static const int kSymbolFieldNumber = 1;
  const ::std::string& symbol() const;
  void set_symbol(const ::std::string& value);
  #if LANG_CXX11
  void set_symbol(::std::string&& value);
  #endif
  void set_symbol(const char* value);
  void set_symbol(const char* value, size_t size);
  ::std::string* mutable_symbol();
  ::std::string* release_symbol();
  void set_allocated_symbol(::std::string* symbol);

  // string sellOrderId = 2;
  void clear_sellorderid();
  static const int kSellOrderIdFieldNumber = 2;
  const ::std::string& sellorderid() const;
  void set_sellorderid(const ::std::string& value);
  #if LANG_CXX11
  void set_sellorderid(::std::string&& value);
  #endif
  void set_sellorderid(const char* value);
  void set_sellorderid(const char* value, size_t size);
  ::std::string* mutable_sellorderid();
  ::std::string* release_sellorderid();
  void set_allocated_sellorderid(::std::string* sellorderid);

  // string buyOrderId = 5;
  void clear_buyorderid();
  static const int kBuyOrderIdFieldNumber = 5;
  const ::std::string& buyorderid() const;
  void set_buyorderid(const ::std::string& value);
  #if LANG_CXX11
  void set_buyorderid(::std::string&& value);
  #endif
  void set_buyorderid(const char* value);
  void set_buyorderid(const char* value, size_t size);
  ::std::string* mutable_buyorderid();
  ::std::string* release_buyorderid();
  void set_allocated_buyorderid(::std::string* buyorderid);

  // double askPrice = 3;
  void clear_askprice();
  static const int kAskPriceFieldNumber = 3;
  double askprice() const;
  void set_askprice(double value);

  // uint64 matchedQuantity = 4;
  void clear_matchedquantity();
  static const int kMatchedQuantityFieldNumber = 4;
  ::google::protobuf::uint64 matchedquantity() const;
  void set_matchedquantity(::google::protobuf::uint64 value);

  // double bidPrice = 6;
  void clear_bidprice();
  static const int kBidPriceFieldNumber = 6;
  double bidprice() const;
  void set_bidprice(double value);

  // @@protoc_insertion_point(class_scope:message.OrderMatch)
 private:

  ::google::protobuf::internal::InternalMetadataWithArena _internal_metadata_;
  ::google::protobuf::internal::ArenaStringPtr symbol_;
  ::google::protobuf::internal::ArenaStringPtr sellorderid_;
  ::google::protobuf::internal::ArenaStringPtr buyorderid_;
  double askprice_;
  ::google::protobuf::uint64 matchedquantity_;
  double bidprice_;
  mutable ::google::protobuf::internal::CachedSize _cached_size_;
  friend struct ::protobuf_OrderMatch_2eproto::TableStruct;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// OrderMatch

// string symbol = 1;
inline void OrderMatch::clear_symbol() {
  symbol_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& OrderMatch::symbol() const {
  // @@protoc_insertion_point(field_get:message.OrderMatch.symbol)
  return symbol_.GetNoArena();
}
inline void OrderMatch::set_symbol(const ::std::string& value) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:message.OrderMatch.symbol)
}
#if LANG_CXX11
inline void OrderMatch::set_symbol(::std::string&& value) {
  
  symbol_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:message.OrderMatch.symbol)
}
#endif
inline void OrderMatch::set_symbol(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:message.OrderMatch.symbol)
}
inline void OrderMatch::set_symbol(const char* value, size_t size) {
  
  symbol_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:message.OrderMatch.symbol)
}
inline ::std::string* OrderMatch::mutable_symbol() {
  
  // @@protoc_insertion_point(field_mutable:message.OrderMatch.symbol)
  return symbol_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* OrderMatch::release_symbol() {
  // @@protoc_insertion_point(field_release:message.OrderMatch.symbol)
  
  return symbol_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OrderMatch::set_allocated_symbol(::std::string* symbol) {
  if (symbol != NULL) {
    
  } else {
    
  }
  symbol_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), symbol);
  // @@protoc_insertion_point(field_set_allocated:message.OrderMatch.symbol)
}

// string sellOrderId = 2;
inline void OrderMatch::clear_sellorderid() {
  sellorderid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& OrderMatch::sellorderid() const {
  // @@protoc_insertion_point(field_get:message.OrderMatch.sellOrderId)
  return sellorderid_.GetNoArena();
}
inline void OrderMatch::set_sellorderid(const ::std::string& value) {
  
  sellorderid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:message.OrderMatch.sellOrderId)
}
#if LANG_CXX11
inline void OrderMatch::set_sellorderid(::std::string&& value) {
  
  sellorderid_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:message.OrderMatch.sellOrderId)
}
#endif
inline void OrderMatch::set_sellorderid(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  sellorderid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:message.OrderMatch.sellOrderId)
}
inline void OrderMatch::set_sellorderid(const char* value, size_t size) {
  
  sellorderid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:message.OrderMatch.sellOrderId)
}
inline ::std::string* OrderMatch::mutable_sellorderid() {
  
  // @@protoc_insertion_point(field_mutable:message.OrderMatch.sellOrderId)
  return sellorderid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* OrderMatch::release_sellorderid() {
  // @@protoc_insertion_point(field_release:message.OrderMatch.sellOrderId)
  
  return sellorderid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OrderMatch::set_allocated_sellorderid(::std::string* sellorderid) {
  if (sellorderid != NULL) {
    
  } else {
    
  }
  sellorderid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), sellorderid);
  // @@protoc_insertion_point(field_set_allocated:message.OrderMatch.sellOrderId)
}

// double askPrice = 3;
inline void OrderMatch::clear_askprice() {
  askprice_ = 0;
}
inline double OrderMatch::askprice() const {
  // @@protoc_insertion_point(field_get:message.OrderMatch.askPrice)
  return askprice_;
}
inline void OrderMatch::set_askprice(double value) {
  
  askprice_ = value;
  // @@protoc_insertion_point(field_set:message.OrderMatch.askPrice)
}

// uint64 matchedQuantity = 4;
inline void OrderMatch::clear_matchedquantity() {
  matchedquantity_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 OrderMatch::matchedquantity() const {
  // @@protoc_insertion_point(field_get:message.OrderMatch.matchedQuantity)
  return matchedquantity_;
}
inline void OrderMatch::set_matchedquantity(::google::protobuf::uint64 value) {
  
  matchedquantity_ = value;
  // @@protoc_insertion_point(field_set:message.OrderMatch.matchedQuantity)
}

// string buyOrderId = 5;
inline void OrderMatch::clear_buyorderid() {
  buyorderid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& OrderMatch::buyorderid() const {
  // @@protoc_insertion_point(field_get:message.OrderMatch.buyOrderId)
  return buyorderid_.GetNoArena();
}
inline void OrderMatch::set_buyorderid(const ::std::string& value) {
  
  buyorderid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:message.OrderMatch.buyOrderId)
}
#if LANG_CXX11
inline void OrderMatch::set_buyorderid(::std::string&& value) {
  
  buyorderid_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:message.OrderMatch.buyOrderId)
}
#endif
inline void OrderMatch::set_buyorderid(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  
  buyorderid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:message.OrderMatch.buyOrderId)
}
inline void OrderMatch::set_buyorderid(const char* value, size_t size) {
  
  buyorderid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:message.OrderMatch.buyOrderId)
}
inline ::std::string* OrderMatch::mutable_buyorderid() {
  
  // @@protoc_insertion_point(field_mutable:message.OrderMatch.buyOrderId)
  return buyorderid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* OrderMatch::release_buyorderid() {
  // @@protoc_insertion_point(field_release:message.OrderMatch.buyOrderId)
  
  return buyorderid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void OrderMatch::set_allocated_buyorderid(::std::string* buyorderid) {
  if (buyorderid != NULL) {
    
  } else {
    
  }
  buyorderid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), buyorderid);
  // @@protoc_insertion_point(field_set_allocated:message.OrderMatch.buyOrderId)
}

// double bidPrice = 6;
inline void OrderMatch::clear_bidprice() {
  bidprice_ = 0;
}
inline double OrderMatch::bidprice() const {
  // @@protoc_insertion_point(field_get:message.OrderMatch.bidPrice)
  return bidprice_;
}
inline void OrderMatch::set_bidprice(double value) {
  
  bidprice_ = value;
  // @@protoc_insertion_point(field_set:message.OrderMatch.bidPrice)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace message

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_INCLUDED_OrderMatch_2eproto
