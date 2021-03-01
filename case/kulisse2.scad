$fn=50;

height=25;
width=100;

drill=0.5;
flat=5;

difference() {    
    rounded_square([width,height],[5,5,5,5], true);    
    translate([0,0]) circle(drill);
    
    translate([1.2,0]) {
        translate([17.6,0]) circle(drill);
        translate([34.2,0]) circle(drill);
        translate([-18,0]) circle(drill);
        translate([-35,0]) circle(drill);
    }
    
    translate([-45,-7.5]) circle(drill);    
    translate([45,-7.5]) circle(drill);
    
    translate([-27,-13.5]) rounded_square([13,4],[0,0,1,1], true);   
    
}

//translate([0,6/2 - drill + flat]) square([width,6], true);

module rounded_square(dim, corners=[10,10,10,10], center=false){
  w=dim[0];
  h=dim[1];

  if (center){
    translate([-w/2, -h/2])
    rounded_square_(dim, corners=corners);
  }else{
    rounded_square_(dim, corners=corners);
  }
}

module rounded_square_(dim, corners, center=false){
  w=dim[0];
  h=dim[1];
  render(){
    difference(){
      square([w,h]);

      if (corners[0])
        square([corners[0], corners[0]]);

      if (corners[1])
        translate([w-corners[1],0])
        square([corners[1], corners[1]]);

      if (corners[2])
        translate([0,h-corners[2]])
        square([corners[2], corners[2]]);

      if (corners[3])
        translate([w-corners[3], h-corners[3]])
        square([corners[3], corners[3]]);
    }

    if (corners[0])
      translate([corners[0], corners[0]])
      intersection(){
        circle(r=corners[0]);
        translate([-corners[0], -corners[0]])
        square([corners[0], corners[0]]);
      }

    if (corners[1])
      translate([w-corners[1], corners[1]])
      intersection(){
        circle(r=corners[1]);
        translate([0, -corners[1]])
        square([corners[1], corners[1]]);
      }

    if (corners[2])
      translate([corners[2], h-corners[2]])
      intersection(){
        circle(r=corners[2]);
        translate([-corners[2], 0])
        square([corners[2], corners[2]]);
      }

    if (corners[3])
      translate([w-corners[3], h-corners[3]])
      intersection(){
        circle(r=corners[3]);
        square([corners[3], corners[3]]);
      }
  }
}