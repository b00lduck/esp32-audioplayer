width = 85;
height = 15;

$fn=100;

pl = 1.5; // stärke grundplatte

bd = 17; // button depth

dx = 17.25; // knopfabstand
difference() {
    translate([-width/2,-height/2,-pl]) cube([width,height,pl]);
    for(i=[-1.5:1:1.5]) {
    translate([dx*i-1.5,3,0]) cube([0.8,15,5], true);
    translate([dx*i+1.5,-3,0]) cube([0.8,15,5], true);
    }
}

translate([-dx*2,0,0]) button1();
translate([-dx*1,0,0]) button2();
button3();
translate([dx*1,0,0]) button4();
translate([dx*2,0,0]) button5();

module button1() {
    translate([0,0,0]) linear_extrude(bd) polygon([[-5,2],[5,2],[5,-2],[-5,-2]]);
    } 

module button2() {
    translate([-5,0,0]) linear_extrude(bd) polygon([[0,0],[5,5],[5,-5]]);
    translate([0,0,0]) linear_extrude(bd) polygon([[0,0],[5,5],[5,-5]]);
} 

module button3() {
    cylinder(bd,5,5);
} 

module button4() {
    translate([0,0,0]) linear_extrude(bd) rotate(180) polygon([[0,0],[5,5],[5,-5]]);
    translate([5,0,0]) linear_extrude(bd) rotate(180) polygon([[0,0],[5,5],[5,-5]]);
} 

module button5() {
    translate([0,0,0]) linear_extrude(bd) polygon([[-5,2],[-2,2],[-2,5],[2,5],[2,2],[5,2],[5,-2],[2,-2],[2,-5],[-2,-5],[-2,-2],[-5,-2]]);
} 
