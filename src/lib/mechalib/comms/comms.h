#pragma once
#ifndef MECHALIB_COMMS_H
#define MECHALIB_COMMS_H

#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

#ifndef TEAM_ID
#define TEAM_ID 0
#endif

enum COMMS_CONN_ERR {
    COMMS_SUCCESS = 0,
    COMMS_FAILURE = 1,
    COMMS_NO_TEAM_ID = 2,
    COMMS_INVALID_TEAM_ID = 3
};

enum COMMS_MDNS_ERR {
    MDNS_SUCCESS = 0,
    MDNS_FAILURE = 1
};

enum COMMS_CONN_TYPE {
    CONN_FIELD = 0,
    CONN_BRIDGE = 1,
    CONN_AP = 2
};

class Comms {
    protected:
        // Field
        const char* fieldSSID = "MechaLeague-Field";
        const char* fieldPass = "12345678";
        const IPAddress subnet = IPAddress(255, 255, 0, 0);
        const IPAddress gateway = IPAddress(192, 168, 255, 255);
        const IPAddress dns1 = IPAddress(8, 8, 8, 8);
        const IPAddress dns2 = IPAddress(1, 1, 1, 1);
        IPAddress localIP = IPAddress();

        // MDNS
        const char* mdnsFormat = "mechaleague-bot-"; // mechaleague-bot-<team_id>.local

        // Control sockets
        WiFiUDP controlServer;
        const uint controlServerPort = 1010;

        WiFiUDP telemetryServer;
        const uint telemetryServerPort = 1011;

        bool validateTeamID() {
            if (TEAM_ID > 0 && TEAM_ID <= 9999) return true;
            return false;
        }

    public:
        void startMDNS() {
            char mdnsName[sizeof(mdnsFormat) + 4]; // Enough space for "mechaleague-bot-9999\0"
            snprintf(mdnsName, sizeof(mdnsName), "%s%d", mdnsFormat, TEAM_ID); // mechaleague-bot-<team_id>
            MDNS.begin(mdnsName);
        }

        void startServer() {
            controlServer.begin(controlServerPort);
            telemetryServer.begin(telemetryServerPort);
        }

        COMMS_CONN_ERR connectToField() {
            return connectToWiFi(fieldSSID, fieldPass);
        }

        static char* generateIPAddress(int teamID) {
            int part1 = (teamID / 100) % 100; // First two digits
            int part2 = teamID % 100;         // Last two digits
            static char ipAddress[16]; // Static ensures it persists after function returns
            snprintf(ipAddress, sizeof(ipAddress), "192.168.%d.%d", part1, part2);
            return ipAddress;
        }

        COMMS_CONN_ERR connectToWiFi(const char* ssid, const char* password = NULL) {
            #ifdef TEAM_ID
            // Validate team ID
            if (!validateTeamID()) return COMMS_CONN_ERR::COMMS_INVALID_TEAM_ID;

            // Compute IP address if needed
            // if (localIP) localIP.fromString(generateIPAddress(TEAM_ID));

            // Connect to wifi
            // WiFi.config(localIP, gateway, subnet, dns1, dns2);
            WiFi.mode(WIFI_STA);
            if (password != NULL) WiFi.begin(ssid, password);
            else WiFi.begin(ssid);
            WiFi.waitForConnectResult();
            wl_status_t status = WiFi.status();

            // Start control and telemetry servers
            startServer();

            // Start MDNS
            startMDNS();

            return status == WL_CONNECTED ? COMMS_CONN_ERR::COMMS_SUCCESS : COMMS_CONN_ERR::COMMS_FAILURE;

            #else
            return COMMS_CONN_ERR::COMMS_NO_TEAM_ID;
            #endif
        }

    // Singleton
    private:
        bool init = false;
    public:
        Comms() {};
        static Comms& getInstance() {
            static Comms instance;
            if (!instance.init) {
                instance.init = true;
                instance.localIP.fromString(Comms::generateIPAddress(TEAM_ID));
            }
            return instance;
        }
};

#endif