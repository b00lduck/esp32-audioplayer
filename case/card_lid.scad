$fn  = 50;

width = 40;
height = 15;

// lid
lidThickness = 3;
pinLength = 2.4;
pinWidth = 5;
pinThickness = lidThickness / 2;
boxToLidSpacing = 0.5;
lidReinforcementDepth = 2;

screwHoleHeadDiameter = 6;
screwHoleHeadDepth = 2;
screwHoleDiameter = 3.2;
screwHoleDistance = 6;

boxOverlap = 10;
boxThickness = 1.2;
boxDepth = 10;
boxKragen = 1.6;
boxFloorThickness = 3;

screwPoleDiameter = 7;

woodWidth = 4;


lid();

//box();

module solidBox() {
    
    // kragen
    translate([0,0,-boxThickness/2]) cube([width + boxOverlap, height + boxOverlap, boxThickness], true);
 
    // durchführung
    translate([0, 0, (woodWidth)/2]) cube([width + boxThickness * 2, height + boxThickness * 2, woodWidth], true);
 
    // rest
    translate([0, 0, (boxDepth-woodWidth)/2 + woodWidth]) cube([width + (boxThickness - boxKragen) * 2, height + (boxThickness - boxKragen) * 2, boxDepth - woodWidth], true);
 
 
}

module hollowBox() {   
   
   difference() { 
      solidBox();
             
      // oberer teil
      translate([-width/2,-height/2,-boxThickness]) cube([width, height, lidThickness]);    
      
      // unterer teil
      translate([0,0,boxDepth/2 - (boxFloorThickness-boxThickness)]) cube([width - boxKragen * 2, height - boxKragen * 2, boxDepth - boxThickness], true);    
       
       // pin slots
       pins();  
        
       // sd card slot
       translate([-4,0,boxDepth-boxFloorThickness/2]) cube([25,3,boxFloorThickness], true); 
   }   

   
}

module hollowBoxWithScrewPost() {
    difference() {
        hollowBox();
        translate([width/2-screwHoleDistance,0, lidThickness - boxThickness]) {
            cylinder(boxDepth-lidThickness, 2, 2);           
        }
    }

    translate([width/2-screwHoleDistance,0, lidThickness - boxThickness]) {
        difference() {
            screwPost();
            cylinder(boxDepth-lidThickness, 2, 2);           
        }    
   }
}

module box() {
    
    difference() {
        hollowBoxWithScrewPost();
        
        // M3 nut
        translate([width/2-screwHoleDistance,0,boxDepth]) 
        linear_extrude(height = 2.5, center = true) polygon([
            [3.17543,0],
            [1.58771,-2.75],
            [-1.58771,-2.75],
            [-3.17543,0],
            [-1.58771,2.75],
            [1.58771,2.75]
        ]);
        
        // notch for opening lid
        translate([width/2,-5,-2.2]) rotate([60,0,90]) cube([10,3.4,2]);      
    }
    
}


module screwPost() {
    translate([0,0,lidReinforcementDepth]) cylinder(boxDepth-lidThickness-lidReinforcementDepth, screwPoleDiameter/2, screwPoleDiameter/2);
    translate([0,0,6.2]) cube([9,13,3], true);
}

module lidWithReinforcement() {
    translate([0,0,0]) {
        cube([width - boxToLidSpacing, height - boxToLidSpacing, lidThickness]);
        translate([screwHoleDistance, height/2, lidThickness]) cylinder(lidReinforcementDepth, screwPoleDiameter/2, screwPoleDiameter/2);
    }
}

module lid() {
    rotate([0,0,180]) translate([-width/2+boxToLidSpacing/2,-height/2+boxToLidSpacing/2,-boxThickness]) {
        difference() {
            lidWithReinforcement();
            translate([screwHoleDistance, height/2, 0]) cylinder(lidThickness + lidReinforcementDepth, screwHoleDiameter/2, screwHoleDiameter/2);
            translate([screwHoleDistance, height/2, 0]) cylinder(screwHoleHeadDepth, screwHoleHeadDiameter/2, screwHoleHeadDiameter/2);
       }
    }
    translate([0,0,0]) pins(boxToLidSpacing);
 }
 
 module pins(spacing = 0) {
    for(i = [-1,1]) 
        translate([(-pinLength-width)/2 , - pinWidth/2 + i*height/4, lidThickness/2 - boxThickness]) {
            cube([pinLength, pinWidth - spacing, pinThickness - spacing]);
        }
}