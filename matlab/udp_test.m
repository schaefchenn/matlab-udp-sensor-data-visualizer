clear
u = udpport("LocalHost","192.168.4.2","LocalPort",5005);

disp("Warte auf UDP...")

while true
    if u.NumDatagramsAvailable > 0
        d = read(u,1,"string");
        disp(d.Data)
    end
    pause(0.1)
end