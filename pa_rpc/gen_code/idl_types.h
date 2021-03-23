/**
 * Autogenerated by Thrift Compiler (0.14.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef idl_TYPES_H
#define idl_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>




class user_info;

class stuff_detail;

class user_apply;

class stuff_plan;

typedef struct _user_info__isset {
  _user_info__isset() : user_id(false), name(false), logo(false), company(false), buyer(false), phone(false) {}
  bool user_id :1;
  bool name :1;
  bool logo :1;
  bool company :1;
  bool buyer :1;
  bool phone :1;
} _user_info__isset;

class user_info : public virtual ::apache::thrift::TBase {
 public:

  user_info(const user_info&);
  user_info& operator=(const user_info&);
  user_info() : user_id(0), name(), logo(), company(), buyer(0), phone() {
  }

  virtual ~user_info() noexcept;
  int64_t user_id;
  std::string name;
  std::string logo;
  std::string company;
  bool buyer;
  std::string phone;

  _user_info__isset __isset;

  void __set_user_id(const int64_t val);

  void __set_name(const std::string& val);

  void __set_logo(const std::string& val);

  void __set_company(const std::string& val);

  void __set_buyer(const bool val);

  void __set_phone(const std::string& val);

  bool operator == (const user_info & rhs) const
  {
    if (!(user_id == rhs.user_id))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(logo == rhs.logo))
      return false;
    if (!(company == rhs.company))
      return false;
    if (!(buyer == rhs.buyer))
      return false;
    if (!(phone == rhs.phone))
      return false;
    return true;
  }
  bool operator != (const user_info &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const user_info & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(user_info &a, user_info &b);

std::ostream& operator<<(std::ostream& out, const user_info& obj);

typedef struct _stuff_detail__isset {
  _stuff_detail__isset() : name(false), last(false), price(false), company(false), type_id(false) {}
  bool name :1;
  bool last :1;
  bool price :1;
  bool company :1;
  bool type_id :1;
} _stuff_detail__isset;

class stuff_detail : public virtual ::apache::thrift::TBase {
 public:

  stuff_detail(const stuff_detail&);
  stuff_detail& operator=(const stuff_detail&);
  stuff_detail() : name(), last(), price(0), company(), type_id(0) {
  }

  virtual ~stuff_detail() noexcept;
  std::string name;
  std::string last;
  int64_t price;
  std::string company;
  int64_t type_id;

  _stuff_detail__isset __isset;

  void __set_name(const std::string& val);

  void __set_last(const std::string& val);

  void __set_price(const int64_t val);

  void __set_company(const std::string& val);

  void __set_type_id(const int64_t val);

  bool operator == (const stuff_detail & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(last == rhs.last))
      return false;
    if (!(price == rhs.price))
      return false;
    if (!(company == rhs.company))
      return false;
    if (!(type_id == rhs.type_id))
      return false;
    return true;
  }
  bool operator != (const stuff_detail &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_detail & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(stuff_detail &a, stuff_detail &b);

std::ostream& operator<<(std::ostream& out, const stuff_detail& obj);

typedef struct _user_apply__isset {
  _user_apply__isset() : name(false), phone(false), logo(false), apply_id(false), status(false) {}
  bool name :1;
  bool phone :1;
  bool logo :1;
  bool apply_id :1;
  bool status :1;
} _user_apply__isset;

class user_apply : public virtual ::apache::thrift::TBase {
 public:

  user_apply(const user_apply&);
  user_apply& operator=(const user_apply&);
  user_apply() : name(), phone(), logo(), apply_id(0), status(0) {
  }

  virtual ~user_apply() noexcept;
  std::string name;
  std::string phone;
  std::string logo;
  int64_t apply_id;
  int64_t status;

  _user_apply__isset __isset;

  void __set_name(const std::string& val);

  void __set_phone(const std::string& val);

  void __set_logo(const std::string& val);

  void __set_apply_id(const int64_t val);

  void __set_status(const int64_t val);

  bool operator == (const user_apply & rhs) const
  {
    if (!(name == rhs.name))
      return false;
    if (!(phone == rhs.phone))
      return false;
    if (!(logo == rhs.logo))
      return false;
    if (!(apply_id == rhs.apply_id))
      return false;
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const user_apply &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const user_apply & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(user_apply &a, user_apply &b);

std::ostream& operator<<(std::ostream& out, const user_apply& obj);

typedef struct _stuff_plan__isset {
  _stuff_plan__isset() : type_id(false), count(false), vichele_info(false), plan_id(false), created_by(false), plan_time(false), created_time(false), name(false), price(false), status(false) {}
  bool type_id :1;
  bool count :1;
  bool vichele_info :1;
  bool plan_id :1;
  bool created_by :1;
  bool plan_time :1;
  bool created_time :1;
  bool name :1;
  bool price :1;
  bool status :1;
} _stuff_plan__isset;

class stuff_plan : public virtual ::apache::thrift::TBase {
 public:

  stuff_plan(const stuff_plan&);
  stuff_plan& operator=(const stuff_plan&);
  stuff_plan() : type_id(0), count(0), plan_id(0), created_by(0), plan_time(), created_time(0), name(), price(0), status(0) {
  }

  virtual ~stuff_plan() noexcept;
  int64_t type_id;
  double count;
  std::vector<std::string>  vichele_info;
  int64_t plan_id;
  int64_t created_by;
  std::string plan_time;
  int64_t created_time;
  std::string name;
  double price;
  int64_t status;

  _stuff_plan__isset __isset;

  void __set_type_id(const int64_t val);

  void __set_count(const double val);

  void __set_vichele_info(const std::vector<std::string> & val);

  void __set_plan_id(const int64_t val);

  void __set_created_by(const int64_t val);

  void __set_plan_time(const std::string& val);

  void __set_created_time(const int64_t val);

  void __set_name(const std::string& val);

  void __set_price(const double val);

  void __set_status(const int64_t val);

  bool operator == (const stuff_plan & rhs) const
  {
    if (!(type_id == rhs.type_id))
      return false;
    if (!(count == rhs.count))
      return false;
    if (!(vichele_info == rhs.vichele_info))
      return false;
    if (!(plan_id == rhs.plan_id))
      return false;
    if (!(created_by == rhs.created_by))
      return false;
    if (!(plan_time == rhs.plan_time))
      return false;
    if (!(created_time == rhs.created_time))
      return false;
    if (!(name == rhs.name))
      return false;
    if (!(price == rhs.price))
      return false;
    if (!(status == rhs.status))
      return false;
    return true;
  }
  bool operator != (const stuff_plan &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const stuff_plan & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(stuff_plan &a, stuff_plan &b);

std::ostream& operator<<(std::ostream& out, const stuff_plan& obj);



#endif
