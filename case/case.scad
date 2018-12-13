include <speakers.scad>;

$fn = 150;

width = 185;
depth = 110;
height = 110;

// 3.6 mm Pappel settings
material = 3.6;
kerf = 0.07;
kerfExtraToothLen = 0.2;


buttonDiameter = 24; 
//outerButtonDiameter = 34;
buttonSpread = 36;
buttonSpacing = 25;
buttonFoo = 0;

speakerHoleDistance = 60.5;
speakerDiameter = 70;
speakerSluts = 2.5;

fingersA = 9;
fingersB = 4;

edgeHoleDistance = 11.6; // 11mm + ?

//dimensions();

/*
    Material dimensions needed:
                                       
    1   top/bottom      240mm x 200mm   
    2   front/back      240mm x 200mm
    3   left/right      240mm x 120mm
    4   speaker ring    210mm x 110mm


    Cost for two cases without room for error:

    Rauchglas braun getoent     5mm     30€ (without rings)
    Rauchglas braun getoent     3mm     27€
    hellblau 5C18               3mm     30€
    Transparent                 5mm     21€ (without rings)
    Transparent                 3mm     21€
    Transparent kratzfest       3mm     29€
    grau 7C83                   3mm     31€ 
       
    https://www.plattenzuschnitt24.de/Acrylglas-Zuschnitt/Acrylglas-getoent/
    https://acrylglasplattenshop.de/acrylglas-getoent/       
    https://expresszuschnitt.de/3mm-PLEXIGLAS-hellgrau-GS-nach-Mass 
*/

color([1,0,0]) {
    //sheet1(); // front/bottom
    sheet2(); // top/back
    //sheet3(); // left/right
    //sheet4(); // speaker ring
    //kerftest();
}

module kerftest() {   
    sq = 40;    
    translate([0,-21,0]) {
        reducedSquare(sq, sq);
        fingersA(40, 40, 3, 0.5);
        fingersB(40, 40, 3, 0, true);
    }

    translate([0,21,0]) {
        reducedSquare(sq, sq);
        fingersA(40, 40, 3, 0);
        fingersB(40, 40, 3, 0);
    }
}

module dimensions() {
    difference() {
        square([211,111],true);
        square([210,110],true);
    }
}

module sheet1() {
    // front
    translate([60,0]) rotate([0,0,90]) faceA(width, depth);

    // bottom
    translate([-60,0,0]) rotate([0,0,90]) difference() {
        faceB(width, depth);
        translate([width/2 - 45.8,height/2 - 32,0]) circle(1.8);
        translate([-width/2 + 45.8,height/2 - 32,0]) circle(1.8);
        translate([width/2 - 45.8,height/2 - 98.8,0]) circle(1.8);
        translate([-width/2 + 45.8,height/2 - 98.8,0]) circle(1.8);
    }
}

module sheet2() {
    translate([-60,0,0]) rotate([0,0,0]) difference() {
        faceA(width, depth);
        translate([-10, 6]) sdCut();
        translate([55,14]) usbCut();
        translate([-65,17]) powerCut();
        translate([width/2 - 25.5,-height/2 + 27]) phoneCut();
    }
/*
    translate([60,0,0]) rotate([0,0,90]) difference() {
        faceB(width, depth);
        translate([-buttonSpread,-buttonSpacing+buttonFoo]) circle(buttonDiameter/2);
        translate([0,-buttonSpacing]) circle(buttonDiameter/2);
        translate([buttonSpread,-buttonSpacing+buttonFoo]) circle(buttonDiameter/2);
        rfid();
    }
    */
}

module rfid() {    
    distA = 34 / 2;
    distB = 25 / 2;   
    distC = 37 / 2;    
    translate([0,20,0]) {
        // two holes are enough and so the asymmetry of the board will be hidden
        //translate([distC,distA,0]) circle(1.6);
        translate([-distC,distB,0]) circle(1.6);
        translate([distC, -distA,0]) circle(1.6);
        //translate([-distC,-distB,0]) circle(1.6);
    }
}

module phoneCut() {
    circle(5.5);
}

module sdCut() {
    square([43.7 - kerf * 2, 18.7 - kerf * 2]);
}

module usbCut() {
    square([22 - kerf * 2, 11 - kerf * 2]);
}

module powerCut() {
    square([13,19], true);
}

module sheet3() {    
    translate([-60,0,0]) difference() {
        faceC(height, depth);
        rotate([0,0,90]) speakerCuts(speakerHoleDistance, 4, speakerDiameter, speakerSluts - kerf/2);
    }
    translate([60,0,0]) difference() {
        faceC(height, depth);
        rotate([0,0,90]) speakerCuts(speakerHoleDistance, 4, speakerDiameter, speakerSluts - kerf/2);
    }
}

module sheet4() {
    translate([-50,0,0]) {
        speakerRing(speakerHoleDistance, 4);
    }
    translate([50,0,0]) {
        speakerRing(speakerHoleDistance, 4);
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
    reduction = (material - kerf) * 2;
    
    as = -(a-reduction)/2;
    bs = -(b-reduction)/2;
    
    at = -(a/2-material);
    bt = -(b/2-material);
    
    difference() {
        translate([as, bs]) square([a - reduction, b - reduction]);
        for(x = [-1,1]) for(y = [-1,1]) {
            translate([x * (at + edgeHoleDistance),y * (bt + edgeHoleDistance)]) circle(2);
        }
    }
}

// fingers on edge A of reduced square
module fingersA (a, b, num, offset) {    
    stride = a / (num + .5);
    for(x = [-1,1])
        for(i = [0+offset:num-offset]) {
            translate([stride/4 - a/2 + i * stride, x * (b/2 - material/2 + kerf)]) 
                square([stride/2 + kerf * 2, material + kerfExtraToothLen], true);
    }
}

// fingers on edge B of reduced square
module fingersB (a, b, num, offset, cut = false) {   
    stride = b / (num + .5);
    for(x = [-1,1]) {
        difference() {
            for(i = [0+offset:num-offset]) {
                translate([x * (a/2 - material/2 + kerf), stride/4 - b/2 + i * stride]) 
                    square([material + kerfExtraToothLen, stride/2 + kerf * 2], true);
            }
            if (cut) {
                translate([x * a/2, -(b/2 + kerf)]) square([material*2,material*2],true);    
                translate([x * a/2, (b/2 + kerf)]) square([material*2,material*2],true);    
            }
        }
    }
}