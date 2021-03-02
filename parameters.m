
Kp = 0.645; %unitless
Ki = 0.41; %volts/second

step = 7;

out=sim('motorsimulation');
plot(out.simout)
hold on
Kp = 0.645; %unitless
Ki = 0.001; %volts/second
out=sim('motorsimulation');
plot(out.simout)