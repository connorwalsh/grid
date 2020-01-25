#ifndef TEST_DOUBLE_DEVICE_GRID_H
#define TEST_DOUBLE_DEVICE_GRID_H

// use standalone asio header only library rather than boost.asio.
// see https://think-async.com/Asio/AsioAndBoostAsio.html for details.
#define ASIO_STANDALONE

#include <string>
#include <thread>
#include <httplib.h>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <spdlog/spdlog.h>

#include "anemone/io.hpp"
#include "anemone/util/concurrent_queue.hpp"


class BrowserGridDevice : public GridDevice {
public:
  BrowserGridDevice(bool interactive, std::shared_ptr< Queue<bool> > ready)
    : interactive(interactive), ready(ready)
  {
    if (interactive) {
      // set html dir to serve over http
      auto ret = svr.set_base_dir("test/doubles/device/html");
      if (!ret) {
        spdlog::error("the html base dir doesn't exist!");
      }
    }
  };

  virtual void connect(std::string addr) override {
    if (interactive) {
      std::thread t([this, addr] {
                      spdlog::info(addr);
                      svr.listen("localhost", std::stoi(addr));
                    });
      t.detach();

      // set websocket callback handler
      ws_server.set_message_handler([this] (websocketpp::connection_hdl,
                                            websocketpp::server<websocketpp::config::asio>::message_ptr msg) {
                                      handle_ws_msg(msg->get_payload());
                                    });
      
      // start websocket listener
      ws_server.init_asio();
      ws_server.listen(2718);
      ws_server.start_accept();
      ws_server.run();
    } else {
      ready->push(true);
    }
  };
  virtual void listen() override {};

  virtual void turn_off(grid_coordinates_t c) override {};
  virtual void turn_on(grid_coordinates_t c) override {};
  virtual void set(grid_coordinates_t c, unsigned int intensity) override {};

private:
  bool interactive;
  httplib::Server svr;
  websocketpp::server<websocketpp::config::asio> ws_server;
  std::shared_ptr< Queue<bool> > ready;

  void handle_ws_msg(std::string const& msg) {
    
    spdlog::error(msg);
  }
};

#endif