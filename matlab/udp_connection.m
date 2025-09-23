clc;

delete(udpportfind("Type","udpport"))
clear u  % vorhandene Variable löschen

u = udpport("LocalPort",5005);
disp("Warte auf Daten...");

while true
    if u.NumBytesAvailable > 0
        data = readline(u);
        value = data;
        fprintf("Messwert: %s\n", value);
    end
end