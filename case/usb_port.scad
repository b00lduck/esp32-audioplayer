$fn  = 50;

width = 18;
height = 7;

boxOverlap = 10;
boxThickness = 2;
boxDepth = 3;
boxKragen = 1.6;
boxFloorThickness = 3;

woodWidth = 3.6;

clipWidth = 4;
clipNoseThickness = 1.3;
clipLength = 3;

box();

module solidBox() {
    
    // kragen
    translate([0,0,-boxThickness/2]) cube([width + boxOverlap, height + boxOverlap, boxThickness], true);
 
    // durchführung
    translate([0, 0, (boxDepth)/2+2]) cube([width + boxThickness * 2, height + boxThickness * 2, boxDepth + 4], true); 
    
    translate([0,3.5,boxDepth + 2]) cube([width + boxThickness * 2,2.4,4], true); 
    
    translate([0,-3.5,boxDepth + 2]) cube([width + boxThickness * 2,2.4,4], true); 
    
    for(n = [-1,1])
    for(i = [-1,1])
    translate([i * width/(4 + n),n * (height/2 + boxThickness),woodWidth]) rotate([90,0,n*90]) linear_extrude(height = clipWidth, center=true) polygon([
            [0, clipLength],
            [0, 0],
            [clipNoseThickness, 0]        
        ]);  

}


module box() {      
   difference() { 
      solidBox();
             
      // oberer teil
      translate([-width/2,-height/2,-boxThickness]) cube([width, height, boxDepth]);    
            
      // usb slot
      translate([0,0,boxDepth-boxFloorThickness/2]) cube([8,3,20], true); 
      translate([0,1.7,boxDepth + 0.4]) cube([20,1.2,20], true); 
   }   
   
}
