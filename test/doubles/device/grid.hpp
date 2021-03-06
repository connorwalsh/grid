#ifndef TEST_DOUBLE_DEVICE_GRID_H
#define TEST_DOUBLE_DEVICE_GRID_H

// use standalone asio header only library rather than boost.asio.
// see https://think-async.com/Asio/AsioAndBoostAsio.html for details.
#define ASIO_STANDALONE

#include <iostream>

#include <string>
#include <thread>
#include <httplib.h>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

#include "doubles/device/mode.hpp"

#include "anemone/io.hpp"
#include "anemone/util/concurrent_queue.hpp"
#include "anemone/util/wait.hpp"



class BrowserGridDevice : public GridDevice {
public:  
  BrowserGridDevice(test_device::Mode test_mode, unsigned int toggle_wait_ms, std::shared_ptr< Queue<bool> > ready);
  ~BrowserGridDevice();

  // this is important to do such that we can translate any press events (via toggle method)
  // from grid_addr_t to grid_coordinates_t
  void set_layout_context(LayoutContext *l);
  
  virtual void connect(std::string addr) override;
  virtual void listen() override;

  virtual void turn_off(grid_coordinates_t c) override;
  virtual void turn_on(grid_coordinates_t c) override;
  virtual void set(grid_coordinates_t c, unsigned int intensity) override;

  void toggle(grid_addr_t addr);
  void toggle(grid_coordinates_t c);

  unsigned int check_led_level(grid_addr_t addr);
  unsigned int check_led_level(grid_coordinates_t c);
  
  void describe(std::string description);
  void wait();

  /// @brief starts recording incoming led commands
  void start_recording();

  /// @brief stops recording incoming led commands and returns the results
  std::vector<std::pair<grid_coordinates_t, unsigned int> > stop_recording();

  /// @brief take a snapshot of the led values at a location
  unsigned int snapshot_led_at(grid_addr_t addr);

  websocketpp::server<websocketpp::config::asio> ws_server;
  std::set<websocketpp::connection_hdl,std::owner_less<websocketpp::connection_hdl> > connections;
private:
  struct {
    bool headless;
    bool visual;
    bool interactive;
  } mode;

  bool is_recording;
  std::vector<std::pair<grid_coordinates_t, unsigned int> > recording_result;
  
  unsigned int toggle_wait_ms;

  LayoutContext *layout;
  
  httplib::Server svr;
  
  std::shared_ptr< Queue<bool> > ready;
  std::map<int, std::map<int, bool> > is_pressed; // y, x, is_pressed
  std::map<int, std::map<int, unsigned int> > led_level; // y, x, led_level
  
  void send(nlohmann::json j);  
  void on_open(websocketpp::connection_hdl hdl);
  void on_close(websocketpp::connection_hdl hdl);
  void handle_ws_msg(websocketpp::connection_hdl hndl,
                     websocketpp::server<websocketpp::config::asio>::message_ptr msg);
};

#endif
