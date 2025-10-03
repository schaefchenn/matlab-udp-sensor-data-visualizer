clc;
delete(udpportfind("Type","udpport"))
clear u

localPort = 5005;
packetSize = 7 * 4; % 7 floats à 4 Bytes = 28 Bytes

u = udpport("LocalPort", localPort, "Timeout", 1);
disp("Warte auf Daten...");

% Zeitfenster für die Anzeige (in Sekunden)
timeWindow = 10;  

% Arrays für alle Daten
timestamps = [];
rolls  = [];
pitches= [];
yaws   = [];
axVals = [];
ayVals = [];
azVals = [];

% Figuren und Plots vorbereiten
figure;
tiledlayout(2,1);

% Orientierung
nexttile;
h1 = plot(nan, nan, '-r', nan, nan, '-g', nan, nan, '-b');
legend('Roll','Pitch','Yaw');
xlabel('Zeit [s]');
ylabel('Winkel [°]');
grid on;

% Beschleunigung
nexttile;
h2 = plot(nan, nan, '-r', nan, nan, '-g', nan, nan, '-b');
legend('ax','ay','az');
xlabel('Zeit [s]');
ylabel('Beschleunigung [m/s^2]');
grid on;

% Hauptschleife
while ishandle(h1(1))  % solange Fenster offen ist
    if u.NumBytesAvailable >= packetSize
        raw = read(u, packetSize, "uint8");
        vals = typecast(uint8(raw), 'single');

        % Werte extrahieren
        timestamp = vals(1);
        roll  = vals(2);
        pitch = vals(3);
        yaw   = vals(4);
        ax    = vals(5);
        ay    = vals(6);
        az    = vals(7);

        % An Arrays anhängen
        timestamps(end+1) = timestamp;
        rolls(end+1)      = roll;
        pitches(end+1)    = pitch;
        yaws(end+1)       = yaw;
        axVals(end+1)     = ax;
        ayVals(end+1)     = ay;
        azVals(end+1)     = az;

        % Nur Daten innerhalb des Zeitfensters behalten
        tmin = timestamp - timeWindow;
        idx = timestamps >= tmin;

        timestamps = timestamps(idx);
        rolls      = rolls(idx);
        pitches    = pitches(idx);
        yaws       = yaws(idx);
        axVals     = axVals(idx);
        ayVals     = ayVals(idx);
        azVals     = azVals(idx);

        % Plots updaten
        set(h1(1), 'XData', timestamps, 'YData', rolls);
        set(h1(2), 'XData', timestamps, 'YData', pitches);
        set(h1(3), 'XData', timestamps, 'YData', yaws);

        margin = timeWindow * 0.6;
        ax1 = ancestor(h1(1),'axes');   % Achse vom oberen Plot
        xlim(ax1, [tmin timestamp + margin]);
        
        % X-Ticks immer 1s Abstand
        xtick_start = ceil(tmin);  % rundet auf nächste Sekunde
        xtick_end   = floor(timestamp + margin);
        xticks(ax1, xtick_start:1:xtick_end);

        set(h2(1), 'XData', timestamps, 'YData', axVals);
        set(h2(2), 'XData', timestamps, 'YData', ayVals);
        set(h2(3), 'XData', timestamps, 'YData', azVals);

        % Achsenlimit mit etwas Platz rechts
        margin = timeWindow * 0.6;
        ax2 = ancestor(h2(1),'axes');   % Achse vom unteren Plot
        xlim(ax2, [tmin timestamp + margin]);
        xtick_start = ceil(tmin);
        xtick_end   = floor(timestamp + margin);
        xticks(ax2, xtick_start:1:xtick_end);

        drawnow limitrate;
    else
        pause(0.001);
    end
end
