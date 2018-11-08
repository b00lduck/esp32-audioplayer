include <speakers.scad>;

$fn = 50;

width = 185;
depth = 110;
height = 110;
material = 4;
kerf = 0.6;
buttonDiameter = 24;
//buttonDiameter = 34;
buttonSpread = 36;
buttonSpacing = 25;
buttonFoo = 0;

speakerHoleDistance = 60.5;
speakerDiameter = 70;
speakerSluts = 2.5;

fingersA = 9;
fingersB = 4;

//dimensions();

color([1,0,0]) {
    //translate([0,200]) 
    //sheet1();
    sheet2();
    //translate([0,-120]) 
    //sheet3();
    //kerftest();
    //speakerRing(speakerHoleDistance, 4);
}


module kerftest() {
    reducedSquare(40, 40);
    fingersA(40, 40, 3, 0.5);
    fingersB(40, 40, 3, 0, true);

    translate([0,43,0]) {
        reducedSquare(40, 40);
        fingersA(40, 40, 3, 0);
        fingersB(40, 40, 3, 0);
    }
}

module dimensions() {
    difference() {
        square([301,201],true);
        square([300,200],true);
    }
}

module sheet1() {
    translate([-90,0,0]) rotate([0,0,90]) faceA(width, depth);
    translate([25,0]) rotate([0,0,90]) faceB(width, depth);
}

module sheet2() {
    translate([-90,0,0]) rotate([0,0,90]) difference() {
        faceA(width, depth);
        translate([-9, 6]) sdCut();
        translate([55,25]) usbCut();
        translate([-65,30]) powerCut();
    }

    translate([25,0,0]) rotate([0,0,90]) difference() {
        faceB(width, depth);
        translate([-buttonSpread,-buttonSpacing+buttonFoo]) circle(buttonDiameter/2);
        translate([0,-buttonSpacing]) circle(buttonDiameter/2);
        translate([buttonSpread,-buttonSpacing+buttonFoo]) circle(buttonDiameter/2);
    }
}

module sdCut() {
    square([42.4 - kerf * 2 , 17.5 - kerf * 2]);
}

module usbCut() {
    //translate([-12,0]) circle(1.6);
    //translate([12,0]) circle(1.6);
    square([20.4 - kerf * 2 , 9.5 - kerf * 2]);
}

module powerCut() {
    square([13,19], true);
}

module sheet3() {
    translate([-90,-40,0]) difference() {
        faceC(height, depth);
        rotate([0,0,90]) speakerCuts(speakerHoleDistance, 4, speakerDiameter, speakerSluts - kerf/2);
    }
    translate([25,-40,0]) difference() {
        faceC(height, depth);
        rotate([0,0,90]) speakerCuts(speakerHoleDistance, 4, speakerDiameter, speakerSluts - kerf/2);
    }
}

module faceA(a,b) {
    reducedSquare(a, b);
    fingersA(a, b, fingersA, 0);
    fingersB(a, b, fingersB, 0);
}

module faceB(a,b) {
    reducedSquare(a, b);
    fingersA(a, b, fingersA, 0.5);
    fingersB(a, b, fingersB, 0.5);
}

module faceC(a,b) {
    reducedSquare(a, b);
    fingersA(a, b, fingersB, 0.5);
    fingersB(a, b, fingersB, 0, true);        
}

// inner square of face without finger joints
module reducedSquare(a, b) {   
    reduction = (material- kerf) * 2;
    translate([-(a-reduction)/2, -(b-reduction)/2]) 
      square([a - reduction, b - reduction]);
}

// fingers on edge A of reduced square
module fingersA (a, b, num, offset) {    
    stride = a / (num + .5);
    fingerDepth = material - kerf; 
    for(x = [-1,1])
        for(i = [0+offset:num-offset]) {
            translate([stride/4 - a/2 + i * stride, x * (b/2 - fingerDepth/2)]) 
                square([stride/2, fingerDepth], true);
    
    }
}

// fingers on edge B of reduced square
module fingersB (a, b, num, offset, cut = false) {   
    stride = b / (num + .5);
    fingerDepth = material - kerf;     
    for(x = [-1,1]) {
        difference() {
            for(i = [0+offset:num-offset]) {
                translate([x * (a/2 - fingerDepth/2), stride/4 - b/2 + i * stride]) 
                    square([fingerDepth, stride/2], true);
            }
            if (cut) {
                translate([x*a/2,-b/2]) square([fingerDepth*2,fingerDepth*2],true);    
                translate([x*a/2,b/2]) square([fingerDepth*2,fingerDepth*2],true);    
            }
        }
    }
}