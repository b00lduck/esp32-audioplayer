$fn = 50;

m3 = 1.8;
m2 = 1.1;

spacing = 15;
spacingOffset = 0;

// main board spacers
spacerCol(0, 4, 3, m3);
spacerCol(1, 4, 8, m3);
spacerCol(2, 4, 8, m3);

// mp3 board spacers
spacerCol(3, 4, 11, m3);

// amp board spacers
spacerCol(4, 2, 4, m2);

// panel board spacers
spacerCol(5, 2, 6, m3);
spacerCol(6, 2, 4, m3);



module spacerCol(transx, num, height, hole) {
    for(y = [0:num-1]) {
        translate([transx * spacing,y * spacing,0]) spacer(height, hole);
    }    
}

module spacer(len, hole) {
    difference() {
        cylinder(len, hole * 1.8, hole * 1.8);
        cylinder(len, hole, hole);
    }
}