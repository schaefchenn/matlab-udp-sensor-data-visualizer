clc;
delete(udpportfind("Type","udpport"))

localPort = 5005;
packetSize = 7 * 4; % 7 floats à 4 Bytes = 28 Bytes

u = udpport("LocalPort", localPort, "Timeout", 1);
disp("Warte auf Daten...");

% Speicher für Plot
bufferLen = 2000; % wie viele Samples im Plot sichtbar sind
timestamps = zeros(1, bufferLen);
rolls  = zeros(1, bufferLen);
pitches= zeros(1, bufferLen);
yaws   = zeros(1, bufferLen);
axVals = zeros(1, bufferLen);
ayVals = zeros(1, bufferLen);
azVals = zeros(1, bufferLen);

% Figuren und Plots vorbereiten
figure;
tiledlayout(2,1);

nexttile;
hold on;
h1(1) = plot(timestamps, rolls, '-r');
h1(2) = plot(timestamps, pitches, '-g');
h1(3) = plot(timestamps, yaws, '-b');
legend('Roll','Pitch','Yaw');
xlabel('Zeit [s]');
ylabel('Winkel [°]');
grid on;

nexttile;
hold on;
h2(1) = plot(timestamps, axVals, '-r');
h2(2) = plot(timestamps, ayVals, '-g');
h2(3) = plot(timestamps, azVals, '-b');
legend('ax','ay','az');
xlabel('Zeit [s]');
ylabel('Beschleunigung [m/s^2]');
grid on;

% Hauptschleife
while ishandle(h1(1))  % solange Fenster offen ist
    if u.NumBytesAvailable >= packetSize
        raw = read(u, packetSize, "uint8");
        vals = typecast(uint8(raw), 'single');

        timestamp = vals(1);
        roll  = vals(2);
        pitch = vals(3);
        yaw   = vals(4);
        ax    = vals(5);
        ay    = vals(6);
        az    = vals(7);

        % ins Ringpuffer-Array schieben
        timestamps = [timestamps(2:end) timestamp];
        rolls      = [rolls(2:end) roll];
        pitches    = [pitches(2:end) pitch];
        yaws       = [yaws(2:end) yaw];
        axVals     = [axVals(2:end) ax];
        ayVals     = [ayVals(2:end) ay];
        azVals     = [azVals(2:end) az];

        % Plots updaten
        set(h1(1), 'XData', timestamps, 'YData', rolls);
        set(h1(2), 'XData', timestamps, 'YData', pitches);
        set(h1(3), 'XData', timestamps, 'YData', yaws);

        set(h2(1), 'XData', timestamps, 'YData', axVals);
        set(h2(2), 'XData', timestamps, 'YData', ayVals);
        set(h2(3), 'XData', timestamps, 'YData', azVals);

        drawnow limitrate; % schnellere GUI-Aktualisierung
    else
        pause(0.001);
    end
end
