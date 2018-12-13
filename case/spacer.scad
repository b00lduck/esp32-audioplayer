$fn = 50;

hole = 1.8;

spacing = 15;

spacerCol(0,2);
spacerCol(1,6);
spacerCol(2,8);

module spacerCol(transx, height) {
    for(y = [0:7]) {
        translate([transx * spacing,y * spacing,0]) spacer(height);
    }    
}

module spacer(len) {
    difference() {
        cylinder(len, hole + 1.5, hole + 1.5);
        cylinder(len, hole, hole);
    }
}