$fn  = 50;

width = 50;
height = 30;
lidThickness = 2;

pinLength = 2;
pinWidth = 5;
pinThickness = 1;

screwHoleHeadDiameter = 5;
screwHoleHeadDepth = 1;
screwHoleDiameter = 3.1;
screwHoleDistance = 6;

boxOverlap = 10;
boxThickness = lidThickness - pinThickness;
boxDepth = 10;
boxKragen = 1;

boxToLidSpacing = 0.2;

woodWidth = 4;

lid();

//x();

module solidBox() {
    translate([0, 0]) cube([width + boxOverlap, height + boxOverlap, boxThickness], true);
 
    translate([0, 0, boxDepth/2]) cube([width + (boxThickness - boxKragen) * 2, height + (boxThickness - boxKragen) * 2, boxDepth], true);
 
    translate([0, 0, boxDepth / 2 - woodWidth / 2]) cube([width + boxThickness * 2, height + boxThickness * 2, woodWidth + lidThickness], true);
}

module box() {    
    difference() {
        solidBox();
        cube([width, height, 3], true);    
        cube([width - boxKragen * 2, height - boxKragen * 2, boxDepth], true);    
        translate([0,0,boxThickness]) pins();   
        translate([-4,0,0]) cube([25,3,30], true); 
    }        
    translate([width/2-screwHoleDistance,0,boxDepth/2]) {
        difference() {
            cylinder(boxDepth-lidThickness-boxThickness, 4.5, 6, true);
            // M3 nut
            translate([0,0,-2.3])
            linear_extrude(height = 2.5, center = true) polygon([
                [3.17543,0],
                [1.58771,-2.75],
                [-1.58771,-2.75],
                [-3.17543,0],
                [-1.58771,2.75],
                [1.58771,2.75]
            ]);
        }
    }
}

module lid() {
    translate([0,0,(lidThickness - pinThickness)/2]) {
        difference() {
            translate([0,0,-boxToLidSpacing/2])
            cube([width - boxToLidSpacing, height - boxToLidSpacing, lidThickness - boxToLidSpacing], true);
            translate([width/2 - screwHoleDistance, 0, 0]) cylinder(10, screwHoleDiameter/2, screwHoleDiameter/2, true);
            translate([width/2 - screwHoleDistance, 0, - lidThickness + screwHoleHeadDepth]) cylinder(screwHoleHeadDepth * 2, screwHoleHeadDiameter/2, screwHoleHeadDiameter/2, true);
        }
        translate([0,0,(lidThickness - pinThickness)/2]) pins(boxToLidSpacing);
    }
 }
 
 module pins(spacing = 0) {
    for(i = [-1,1]) 
        translate([-width/2 , i*height/4, -spacing]) {
            cube([pinLength, pinWidth - spacing, pinThickness - spacing], true);
        }
}