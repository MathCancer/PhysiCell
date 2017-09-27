camera {
	location < 1.3,  1.0,  0.9 >

	look_at < 1,  1,  1 >
	rotate  <0, 0, 0>

}
background {
	color rgb < 1.0,  1.0,  1.0 >
}
light_source {
	  < 2.0,  1.0,  0.38372093023255816 >
	color rgb < 1.0,  1.0,  1.0 >
}
light_source {
	  < 2.0,  -1.0,  0.38372093023255816 >
	color rgb < 1.0,  1.0,  1.0 >
}

light_source {
	  < 2.0,  1.0,  -0.38372093023255816 >
	color rgb < 1.0,  1.0,  1.0 >
}

union {

//#declare crossPlane= <0.46,0.0,0>;
#declare crossPlane= <0,1.2,0>;
#declare visibleFinish = finish { reflection 0.0 phong 0 };
#declare invisibleFinish = finish { phong 0 };
#declare NEC = color rgbt < 0.0 , 0.0 , 1.0>;
#declare NECFinish = visibleFinish;
#declare APOP = color rgb < 1.0 , 1.0 , 0.0 >;
#declare APOPFinish = visibleFinish;
#declare LIVE = color rgb < 0.0 , 0.8 , 0.0 >;
#declare LIVEFinish = visibleFinish;
#declare MISC = color rgb < 1.0 , 0.0 , 0.0 >;
#declare MISCFinish = visibleFinish;

#macro FinishMacro(loc, fin, col)
  #if(loc.x>=crossPlane.x & loc.y>=crossPlane.y & loc.z>=crossPlane.z)
	  pigment { col }
	  finish{ fin }
  #else
	  pigment { rgbt<1,1,1,1> }
	  finish { invisibleFinish }
  #end
#end