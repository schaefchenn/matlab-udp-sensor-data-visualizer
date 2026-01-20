clc; clear;

u = udpportfind;
if isempty(u)
    localPort = 5005;
    localHost = "192.168.2.210";
    tag = "matlab";

    u = udpport("LocalPort",localPort,"LocalHost",localHost, "Tag",tag);
end

packetSize = 7 * 4;

figure(1)
% Initialize the plot for real-time data visualization
hold on;
h1(1) = plot(0, 0, '-r');
h1(2) = plot(0, 0, '-g');
h1(3) = plot(0, 0, '-b');
legend('Roll','Pitch','Yaw');
xlabel('Time [s]');
ylabel('Angle [°]');
grid on;

%% Allgemeine Einstellungen
windowPoints = 200; % Anzahl Punkte im Fenster

% Ringpuffer vorbereiten
timeData  = zeros(1, windowPoints);

rollData  = zeros(1, windowPoints);
pitchData = zeros(1, windowPoints);
yawData   = zeros(1, windowPoints);

axData    = zeros(1, windowPoints);
ayData    = zeros(1, windowPoints);
azData    = zeros(1, windowPoints);

head = 0; % aktueller Schreibindex
count = 0; % Anzahl gefüllter Punkte

%% Loop
while true
    raw = read(u, packetSize, "uint8");
    vals = typecast(uint8(raw), 'single');

    % Euler-Winkel
    time  = vals(1);
    roll  = vals(2);
    pitch = vals(3);
    yaw   = vals(4);

    % Beschleunigungen
    ax    = vals(5);
    ay    = vals(6);
    az    = vals(7);

    % Ringpuffer-Index
    head = mod(head, windowPoints) + 1;

    % Daten speichern
    timeData(head)  = time;
    rollData(head)  = roll;
    pitchData(head) = pitch;
    yawData(head)   = yaw;

    axData(head) = ax;
    ayData(head) = ay;
    azData(head) = az;

    % Anzahl gefüllter Punkte hochzählen
    if count < windowPoints
        count = count + 1;
    end

    % Ringpuffer in Plot-Reihenfolge umwandeln
    if count == windowPoints
        idx = mod((head+1:head+windowPoints)-1, windowPoints) + 1;
    else
        idx = 1:count;
    end

    %% Update Euler-Winkel Plot
    figure(1)
    h1(1).XData = timeData(idx);
    h1(1).YData = rollData(idx);
    h1(2).XData = timeData(idx);
    h1(2).YData = pitchData(idx);
    h1(3).XData = timeData(idx);
    h1(3).YData = yawData(idx);

    if count == windowPoints
        xlim([timeData(idx(1)), timeData(idx(end))]);
    end

    %% Update Beschleunigungs-Plot
    figure(2)
    h2(1).XData = timeData(idx);
    h2(1).YData = axData(idx);
    h2(2).XData = timeData(idx);
    h2(2).YData = ayData(idx);
    h2(3).XData = timeData(idx);
    h2(3).YData = azData(idx);

    if count == windowPoints
        xlim([timeData(idx(1)), timeData(idx(end))]);
    end

    drawnow limitrate;
end