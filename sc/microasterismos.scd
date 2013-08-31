// --------------------------
// ***   DEVUISON   ***
// --------------------------

// m -> friction sounds Array
// mf -> friction sounds Array
// mr -> bumb sounds Array

//SETUP
(
// read file into buffers
// sounds have to be located in supecollider_app_folder/sounds/
// "Warps" Sounds Array
m = Array.with(Buffer.read(s, "sounds/ritmos_001.wav"), Buffer.read(s, "sounds/space_01.wav"), Buffer.read(s, "sounds/space_02.wav"), Buffer.read(s, "sounds/space_03.wav"), Buffer.read(s, "sounds/space_04.wav"), Buffer.read(s, "sounds/space_05.wav"), Buffer.read(s, "sounds/space_06.wav"), Buffer.read(s, "sounds/space_07.wav"), Buffer.read(s, "sounds/space_04.wav"), Buffer.read(s, "sounds/space_07.wav"), Buffer.read(s, "sounds/space_07.wav"));

d = Array.with(Buffer.read(s, "sounds/ritmos_001.wav"), Buffer.read(s, "sounds/space_01.wav"), Buffer.read(s, "sounds/space_02.wav"), Buffer.read(s, "sounds/space_03.wav"), Buffer.read(s, "sounds/space_04.wav"), Buffer.read(s, "sounds/space_05.wav"), Buffer.read(s, "sounds/space_06.wav"), Buffer.read(s, "sounds/space_07.wav"), Buffer.read(s, "sounds/space_04.wav"), Buffer.read(s, "sounds/space_07.wav"), Buffer.read(s, "sounds/space_07.wav"));

e = Array.with(Buffer.read(s, "sounds/ritmos_001.wav"), Buffer.read(s, "sounds/space_01.wav"), Buffer.read(s, "sounds/space_02.wav"), Buffer.read(s, "sounds/space_03.wav"), Buffer.read(s, "sounds/space_04.wav"), Buffer.read(s, "sounds/space_05.wav"), Buffer.read(s, "sounds/space_06.wav"), Buffer.read(s, "sounds/space_07.wav"), Buffer.read(s, "sounds/space_04.wav"), Buffer.read(s, "sounds/space_07.wav"), Buffer.read(s, "sounds/space_07.wav"));

// "Snap" Sounds Array
n = Array.with(Buffer.read(s, "sounds/golpes_001.wav"), Buffer.read(s, "sounds/golpes_002.wav"), Buffer.read(s, "sounds/golpes_003.wav"), Buffer.read(s, "sounds/golpes_004.wav"), Buffer.read(s, "sounds/golpes_005.wav"), Buffer.read(s, "sounds/golpes_006.wav"), Buffer.read(s, "sounds/golpes_007.wav"), Buffer.read(s, "sounds/golpes_008.wav"), Buffer.read(s, "sounds/golpes_009.wav"), Buffer.read(s, "sounds/golpes_010.wav"));

// Synths List
l = List.new;

// OSC
// Send
y = NetAddr.new("127.0.0.1", 12345);

// Receive
z = NetAddr.new("127.0.0.1", 57120);

// *** Play buffer ** S N A P S **
SynthDef(\playBuf,{ arg out = 0, bufnum, amp=0.7, pan=0.0;
	var signal, env;
	env = EnvGen.ar(Env.perc, doneAction:2); // envelope
	signal = PlayBuf.ar(1, bufnum, BufRateScale.kr(bufnum), doneAction:2) * env;
	signal = Pan2.ar(signal, pan) * amp;
  	Out.ar(out, signal);
}).load(s);

// *** Play buffer ** W A R P S **
// pointer between 0 an 1
// pitch between 0.5 and 2.0 (octave down - octave up)
SynthDef(\warp, {arg out = 0, bufnum, amp=0.7, pan=0.0, pointer = 0.1, pitch = 1.0, gate = 1;
	var signal, env;
	env = EnvGen.ar(Env.asr(0.001, 2, 3, -3), gate);
	signal = Warp1.ar(1, bufnum, Line.kr(0, 1, 15), pitch, 0.1, -1, 8, 0.15, 1.0);
	signal = Pan2.ar(signal, pan) * amp * env;
	Out.ar(out, signal);
}).send(s);

// Receive OSC and play ** S N A P ** the sound
// /collision msg[]:
// msg[1] = true / false
// msg[2] = sample
// msg[3] = vol
// msg[4] = pan
// msg[4].postln;
o = OSCresponder.new(nil, "/collision", { |time, resp, msg|
	if(msg[1] == 'true'){
		x = Synth(\playBuf, [\bufnum, n[msg[2]].bufnum, \amp, 0, \pan, msg[4]]);
	};
}).add;

// Receive OSC and ** KILL ** the synth
// /collision msg[]:
// msg[1] = true / false
// msg[2] = ballId
// msg[3] = soundId
q = OSCresponder.new(nil, "/kill", { |time, resp, msg|
	if(msg[1] == 'true'){
		l[msg[3]].free;
	};
}).add;


// Receive OSC and ** W A R P ** the sound
// /friction msg[]:
// msg[1] = 0 / 1 / 2 (starts / set / stops)
// msg[2] = ballId
// msg[3] = soundId
// msg[4] = sample
// msg[5] = vol
// msg[6] = pan
// msg[7] = pointer
// msg[8] = pitch
// msg[9] = type (choose from the sample arrays from above: m / d / e -> ritmos / frase / friccion)
p = OSCresponder.new(nil, "/friction", { |time, resp, msg|
	if(msg[1] == 'play'){
		if(msg[9] == 'ritmos'){
			w = Synth(\warp, [\bufnum, m[msg[4]].bufnum, \ballid, msg[2], \soundId, msg[3], \amp, msg[5], \pan, msg[6], \pitch, msg[8]]);
		};
		if(msg[9] == 'frase'){
			w = Synth(\warp, [\bufnum, d[msg[4]].bufnum, \ballid, msg[2], \soundId, msg[3], \amp, msg[5], \pan, msg[6], \pitch, msg[8]]);
		};
		if(msg[9] == 'friccion'){
			w = Synth(\warp, [\bufnum, e[msg[4]].bufnum, \ballid, msg[2], \soundId, msg[3], \amp, msg[5], \pan, msg[6], \pitch, msg[8]]);
		};
		l.add(w);
		// send "/soundId": ballId, soundId
		y.sendMsg("/soundId", msg[2], l.size-1);
	};
	if(msg[1] == 'update'){
		l[msg[3]].set(\pan, msg[6], \pointer, msg[7], \pitch, msg[8], \gate, 1);
	};
	if(msg[1] == 'stop'){
		l[msg[3]].set(\gate, 0);
	};
}).add;

)