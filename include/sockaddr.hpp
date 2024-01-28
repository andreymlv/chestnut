#pragma once

#include <QHostAddress>

struct SockAddr {
  QHostAddress ip;
  quint16 port;
};

inline bool operator==(const SockAddr &a, const SockAddr &b) { return a.ip == b.ip && a.port == b.port; }
inline bool operator!=(const SockAddr &a, const SockAddr &b) { return !(a == b); }
inline size_t qHash(const SockAddr &key, size_t seed) { return qHashMulti(seed, key.ip, key.port); }
inline QDebug operator<<(QDebug debug, const SockAddr &addr) {
  QDebugStateSaver saver(debug);
  debug.nospace() << addr.ip << ':' << addr.port;
  return debug;
}
