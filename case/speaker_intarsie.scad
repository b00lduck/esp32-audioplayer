$fn = 150;

width = 185;
depth = 110;
height = 110;

speakerDiameter = 70;
drillSize = 3.2;
step = 4.15;

sheet3(); // left/right

module sheet3() {    
    translate([-52,0,0]) difference() {
        circle(50);
        rotate([0,0,90]) speakerPerforation2(speakerDiameter, drillSize, 5);
    }
    translate([52,0,0]) difference() {
        circle(50);
        rotate([0,0,90]) speakerPerforation2(speakerDiameter, drillSize, 5);
    }
}

module speakerPerforation(diameter, holeRadius, holeDiameter) {
  a = 0;
  for(x=[-diameter/2 + a:holeDiameter:diameter/2]) {         
    for(y=[-diameter/2 + a:holeDiameter:diameter/2]) {
        if (((x*x)+(y*y)) < (diameter/2)*(diameter/2)) {           
            translate([x,y]) circle(holeRadius);          
        }
    }
  }
}   

module speakerPerforation2(diameter, holeDiameter, ringdist) {
    
    for(r=[0:step:diameter/2]) { 
  
        umfang = 6.28*r;
        
        numholes = umfang / 5;
        
        step = 360/round(numholes);        
        
        for(phi=[0:step:360]) {        
            x = sin(phi + r) * r;
            y = cos(phi + r) * r;          
            translate([x,y]) circle(holeDiameter/2);
          
        }
    }
}   