#include "xbox.h"

XboxSeriesXControllerESP32_asukiaaa::Core xboxController;
bool firstConnection = true;
bool wasConnected = false;

void setupXboxController() {
    Serial.println("Connecting Xbox Controller...");
    xboxController.begin();
}

void xboxOnLoop(float* axes, bool* buttons, bool& connected) {
    xboxController.onLoop();

    if (firstConnection) {
        Serial.println("Connecting Xbox Controller...");
        connected = false;
        while (!xboxController.isConnected()) {
            xboxController.onLoop();
            Serial.print(".");
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        vibrate();
        firstConnection = false; // Nur einmal ausführen
        connected = true;
        Serial.println("\nController connected!");

    } else {
        if (xboxController.isConnected()) {
            if (!wasConnected) {
                vibrate();
            }
            
            connected = true;
            // Joysticks & Trigger
            axes[0] = xboxController.xboxNotif.joyLHori;
            axes[1] = xboxController.xboxNotif.joyLVert;
            axes[2] = xboxController.xboxNotif.joyRHori;
            axes[3] = xboxController.xboxNotif.joyRVert;
            axes[4] = xboxController.xboxNotif.trigLT;
            axes[5] = xboxController.xboxNotif.trigRT;

            // Buttons
            buttons[0]  = xboxController.xboxNotif.btnA;
            buttons[1]  = xboxController.xboxNotif.btnB;
            buttons[2]  = xboxController.xboxNotif.btnX;
            buttons[3]  = xboxController.xboxNotif.btnY;
            buttons[4]  = xboxController.xboxNotif.btnLB;
            buttons[5]  = xboxController.xboxNotif.btnRB;
            buttons[6]  = xboxController.xboxNotif.btnSelect;
            buttons[7]  = xboxController.xboxNotif.btnStart;
            buttons[8]  = xboxController.xboxNotif.btnLS;
            buttons[9]  = xboxController.xboxNotif.btnRS;
            buttons[10] = xboxController.xboxNotif.btnDirUp;
            buttons[11] = xboxController.xboxNotif.btnDirDown;
            buttons[12] = xboxController.xboxNotif.btnDirLeft;
            buttons[13] = xboxController.xboxNotif.btnDirRight;
            buttons[14] = xboxController.xboxNotif.btnXbox;
            buttons[15] = xboxController.xboxNotif.btnShare;
        } else {
            connected = false;
            // Controller not connected: all values set to 0 / false
            for (int i = 0; i < 6; i++) axes[i] = 0;
            for (int i = 0; i < 14; i++) buttons[i] = false;

            Serial.println("Controller disconnected!");
            if (xboxController.getCountFailedConnection() > 2) {
                ESP.restart();
            }
        }
    }

    wasConnected = connected;
}

void vibrate() {
    XboxSeriesXHIDReportBuilder_asukiaaa::ReportBase repo;

    repo.v.select.center = true;
    repo.v.select.left = false;
    repo.v.select.right = false;
    repo.v.select.shake = false;
    repo.v.power.center = 80;  // 30% power
    repo.v.timeActive = 50;    // 0.5 second
    Serial.println("run center 30\% power in half second");
    xboxController.writeHIDReport(repo);
    delay(2000);
}
