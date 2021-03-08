$fn = 150;

width = 185;
depth = 110;
height = 110;

speakerDiameter = 70;
drillSize = 2.9;
step = 5.4;

sheet3(); // left/right


ovalX = 99;
ovalY = 100;


module sheet3() {    
    translate([-52,0,0]) difference() {
        scale([ovalX,ovalY]) circle(0.5);
        rotate([0,0,90]) speakerPerforation3(speakerDiameter, drillSize, false);
    }
    translate([52,0,0]) difference() {
        scale([ovalX,ovalY]) circle(0.5);
        rotate([0,0,90]) speakerPerforation3(speakerDiameter, drillSize, true);
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
        numholes = umfang / 5.5;        
        step = 360/round(numholes);        
        for(phi=[0:step:360]) {        
            x = sin(phi + r) * r;
            y = cos(phi + r) * r;          
            translate([x,y]) circle(holeDiameter/2);          
        }
    }
}   

module speakerPerforation3(diameter, slitRadius, direction) {

  turn = direction ? 1 : -1;
  dia = 0;

    
  for(x=[0:45:360]) {             
    for(y=[6:0.3:speakerDiameter/2]) {        
        dia = slitRadius * y/speakerDiameter;
        dia2 = dia < 0.7 ? 0.7 : dia;

        rotate([0,0,turn*(y*2+x)])
        translate([0,y]) circle(dia2);          
    }
  }
  for(x=[22.5:45:360]) {             
    for(y=[14:0.3:speakerDiameter/2]) {        
        dia = slitRadius * y/speakerDiameter;
        dia2 = dia < 0.7 ? 0.7 : dia;        
        rotate([0,0,turn*(y*2+x)])
        translate([0,y]) circle(dia2);          
    }
  }  
  
  for(x=[6:45:360]) {             
    for(y=[0:0.1:2.5]) {        
        dia = slitRadius * y/speakerDiameter * 5;
        dia2 = dia < 0.2 ? 0.2 : dia;        
        rotate([0,0,turn*(y*15+x)])
        translate([0,y]) circle(dia2);          
    }
  }    
  
 
}   
