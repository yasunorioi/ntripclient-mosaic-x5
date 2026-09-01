/*
 *  ntripclient-mosaic-x5 — M5Atom Lite NTRIP client for a Septentrio mosaic-X5 rover.
 *
 *  Thin wrapper over the receiver-agnostic core (ntripclient-core / NtripRover.h).
 *  Same firmware as the G5-P3H build; only the receiver-specific bits differ.
 *
 *  This X5 (fw 4.15.x) is a full RTK Rover+Base unit and — unlike the P3H — answers
 *  ASCII commands (COM input=auto). The Atom's Grove is wired to COM2 here.
 *
 *  Wiring (Grove HY2.0, both sides 3.3V LVTTL):
 *    Atom G26 (TX) ──► mosaic COM2 RX      Atom G32 (RX) ◄── mosaic COM2 TX      GND──GND
 *    RTCM3 over WiFi ──► Atom ──► COM2 (RTK)  ;  COM2 GGA+GSA+GSV ──► Atom (LED + skyplot)
 *  If no fix / no GGA, suspect G26/G32 swapped.
 *
 *  Receiver setup: enable GGA+GSA+GSV on COM2 and save boot config (RxTools/USB):
 *    setNMEAOutput, Stream1, COM2, GGA+GSA+GSV, sec1
 *    exeCopyConfigFile, Current, Boot
 *  (On the shared RTCM+command UART the auto-detect tends to lock to RTCM, so the
 *   Atom's self-provisioning may no-ack; the boot config makes it work regardless.)
 */
#include <NtripRover.h>

void setup() {
  NtripRover::Options o;
  o.board   = "mosaic-X5";
  o.atomCom = "COM2";        // Atom Grove is wired to mosaic COM2 on this unit
  NtripRover::begin(o);
}

void loop() { NtripRover::loop(); }
