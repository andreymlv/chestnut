#pragma once

#include <fmt/ranges.h>

#include <array>
#include <charconv>
#include <cstdint>
#include <string>

class IpAddr {
 public:
  virtual ~IpAddr() = default;

  virtual const std::string to_string() const = 0;
  virtual const bool is_ipv4() const = 0;
  virtual const bool is_ipv6() const = 0;
};

class Ipv4Addr : public IpAddr {
  struct M {
    std::array<uint8_t, 4> octets;
  } m;
  explicit Ipv4Addr(M m) : m(std::move(m)) {}

 public:
  static const auto create(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
    return Ipv4Addr(M{{a, b, c, d}});
  }

  // TODO: rewrite this xD
  static const auto from(std::string_view s) {
    auto to_int = [](std::string_view s) -> uint8_t {
      int value{};
      if (std::from_chars(s.data(), s.data() + s.size(), value).ec ==
          std::errc{})
        return value;
      else
        return -1;
    };

    std::array<size_t, 3> dots{};
    dots[0] = s.find('.', 0);
    dots[1] = s.find('.', dots[0]);
    dots[2] = s.find('.', dots[1]);
    auto a = to_int(s.substr(0, dots[0]));
    return Ipv4Addr(
        M{{to_int(s.substr(0, dots[0])), to_int(s.substr(dots[0], dots[1])),
           to_int(s.substr(dots[1], dots[2])), to_int(s.substr(dots[2]))}});
  }

  const auto octets() const { return m.octets; };

  const std::string to_string() const override {
    return fmt::format("{}", fmt::join(octets(), "."));
  };
  const bool is_ipv4() const override { return true; };
  const bool is_ipv6() const override { return false; };

  static const Ipv4Addr LOCALHOST;
  static const Ipv4Addr UNSPECIFIED;
  static const Ipv4Addr BROADCAST;
};

class Ipv6Addr : public IpAddr {
  struct M {
    std::array<uint8_t, 16> octets;
  } m;
  explicit Ipv6Addr(M m) : m(std::move(m)) {}

 public:
  static const auto create(uint16_t a, uint16_t b, uint16_t c, uint16_t d,
                           uint16_t e, uint16_t f, uint16_t g, uint16_t h) {
    std::array<uint16_t, 8> arr16 = {a, b, c, d, e, f, g};

    std::array<uint8_t, 16> arr8;
    for (int i = 0; i < 8; i++) {
      arr8[i * 2] = arr16[i] & 0xFF;
      arr8[i * 2 + 1] = (arr16[i] >> 8) & 0xFF;
    }

    return Ipv6Addr(M{std::move(arr8)});
  }

  const auto octets() const { return m.octets; };

  const std::string to_string() const override {
    return fmt::format("{}", fmt::join(octets(), "."));
  };
  const bool is_ipv4() const override { return false; };
  const bool is_ipv6() const override { return true; };
};
