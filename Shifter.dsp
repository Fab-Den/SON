import("stdfaust.lib");

modeN = nentry("mode",0,0,2,0); // 0 pour neutre, 1 pour mineur, 2 pour majeur
minor = _+transpose(window,xfade,-4)*0.5+transpose(window,xfade,-7)*0.5;
major = _+transpose(window,xfade,-3)*0.5+transpose(window,xfade,-7)*0.5;
mode(number) = ba.if(number==2, major, ba.if(number==1, minor, _));

ovni = nentry("ovni",0,0,1,1); //smooth par défaut

shift = nentry("shift",1,-20,20,0.001);
transpose_step = ba.if(ovni==1,
                        transpose(window, xfade, shift),
                        transpose(window:si.smoo, xfade:si.smoo, shift:si.smoo));

gain = nentry("gain",0.5,0,1,0.01):si.smoo;
window = nentry("window",100,10,10000,10);
xfade = nentry("xfade",10,1,1000,1);

transpose(w, x, s, sig) = de.fdelay(maxDelay,d,sig)*ma.fmin(d/x,1) +
    de.fdelay(maxDelay,d+w,sig)*(1-ma.fmin(d/x,1))
with {
    maxDelay = 3000;
    i = 1 - pow(2, s/12);
    d = i : (+ : +(w) : fmod(_,w)) ~ _;
};

process = _<:transpose_step*gain<:mode(modeN);