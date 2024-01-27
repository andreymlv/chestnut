#include <fmt/ranges.h>

#include <array>
#include <cstddef>

#include "cxxopts.hpp"
#include "socket.hpp"

int main(int argc, char** argv) {
  cxxopts::Options options("chestnut", "Simple music game");
  options.add_options()

      ("a,address", "IP Address",
       cxxopts::value<std::string>()->default_value("localhost"))

          ("p,port", "Port", cxxopts::value<uint16_t>()->default_value("0"))

              ("h,help", "Print usage");

  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    fmt::println("{}", options.help());
    return 0;
  }

  auto ip = result["address"].as<std::string>();
  auto port = result["port"].as<uint16_t>();

  auto addr = std::make_unique<SocketAddrV4>(Ipv4Addr::LOCALHOST, port);
  auto socket = UdpSocket::bind(std::move(addr)).value();

  std::array<std::byte, 128> buf{};
  auto [amt, src] = socket.recv_from(buf).value();
  fmt::println("[{}]", fmt::join(buf, ", "));

  std::ranges::reverse(buf);
  auto _ = socket.send_to(buf, std::move(src)).value();

  return 0;
}
