# Message - A chat room application with full anonymity.

## dependencies
- [Boost](https://www.boost.org/) >= 1.72.0
  - [Boost.Asio](https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio.html)
  - [Boost.Beast](https://www.boost.org/doc/libs/1_72_0/libs/beast/doc/html/index.html)
- [rapidjson](https://http://rapidjson.org/) >= 1.1.0
- [fmt](https://fmt.dev) >= 6.0.0
- [Qt](https://www.qt.io/) >= 5.12
  - [Qt Widgets](https://doc.qt.io/qt-5/qtwidgets-index.html)
  - [Qt WebSockets](https://doc.qt.io/qt-5/qtwebsockets-index.html)

## build
```
$ cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=DEBUG
$ cmake --build build
```

## Need to do
- [ ] Fix the bug that the client list view cannot be scrolled.
- [ ] Fix the potential security deserialize issue.
- [ ] Add a list view to show online users.
- [ ] Use base64 to encode the message that is sent to the server.
- [ ] Add authentication system to support password login.
- [ ] Use channel to support multiple chat rooms.