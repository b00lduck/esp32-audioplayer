// Speaker grid cutouts

module speakerCuts(holeDist, holeDiameter, diameter, cutWidth) {
    speakerGrid(diameter, cutWidth);
    speakerHoles(holeDist, holeDiameter);
}

module speakerCuts2(holeDist, holeDiameter, diameter, holeRadius) {
    speakerPerforation(diameter, holeRadius, holeRadius * 2.8);
    speakerHoles(holeDist, holeDiameter);
}

module speakerHoles(holeDist,holeDiameter) {     
  for(x=[-holeDist/2,holeDist/2])
      for(y=[-holeDist/2,holeDist/2])
          translate([x,y]) circle(holeDiameter / 2);
}

module speakerGrid(diameter, cutWidth) {          
  for(i=[-diameter/2 + 2.5:5:diameter/2]) {         
     y = (i) / (diameter / 2);            
     phi = acos(y);                     
     lenf = sin(phi) * diameter;
     translate([0,i]) speakerRow(lenf, cutWidth);          
  }
}

module speakerPerforation(diameter, holeRadius, holeDiameter) {
   a = 1.8;
  for(x=[-diameter/2 + a:holeDiameter:diameter/2]) {         
    for(y=[-diameter/2 + a:holeDiameter:diameter/2]) {
        if (((x*x)+(y*y)) < (diameter/2)*(diameter/2)) {           
            translate([x,y]) circle(holeRadius);          
        }
    }
  }
}    

module speakerRow(width, cutWidth) {
  union() {      
    translate([width/2,0]) circle(cutWidth/2);
    translate([-width/2,0]) circle(cutWidth/2);
    square([width,cutWidth],true);          
  }
}

module speakerRing(holeDist, holeDiameter) {
    difference() {
        circle(97/2);
        circle(76/2);
        speakerHoles(holeDist, holeDiameter);
    }
}
