
$fn = 60;
thickness = 3;
size = 16;

hole = 1.9;
reinf = 4.5;

nutThickness = 2.4;

dist = reinf;

rotate([0,90,0])
difference() { 
    foo();    
    translate([dist,dist,size-thickness]) bore();
    translate([dist,size-thickness,dist]) rotate([-90,0,0]) bore();
    translate([size-thickness,dist,dist]) rotate([0,90,0]) bore();
}

module foo() {    
    difference() {
        cube([size,size,size]);
        cube([size-thickness, size-thickness, size-thickness]);
        translate([size - thickness,0,0]) cube([thickness + 1,reinf,reinf]);
        rotate([90,0,90]) translate([size - thickness,0,0]) cube([thickness + 1,reinf,reinf]);
        rotate([-90,-90,0]) translate([size - thickness,0,0]) cube([thickness + 1,reinf,reinf]);
    }    
    boreReinforce();
}

module boreReinforce() {
    g = size - thickness - nutThickness;
    translate([dist, dist, g]) cylinder(thickness+nutThickness, reinf, reinf);
    translate([dist, g, dist]) rotate([-90,0,0]) cylinder(thickness + nutThickness, reinf, reinf);
    translate([g, dist, dist]) rotate([0,90,0]) cylinder(thickness + nutThickness, reinf, reinf);
}

module bore() {
    cylinder(thickness, hole, hole);
    translate([0, 0, -nutThickness-0.3])
    linear_extrude(height = nutThickness + 0.3) scale([1.05,1.05,1.05]) polygon([
        [3.17543,0],
        [1.58771,-2.75],
        [-1.58771,-2.75],
        [-3.17543,0],
        [-1.58771,2.75],
        [1.58771,2.75]
    ]);
}