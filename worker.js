function computeRow(task) 
{
    var iter = 0;
    var cy = task.i;
    var max_iter = task.max_iter;
    var escape = task.escape * task.escape;
    task.values = [];
    
	for (var i = 0; i < task.width; i++)
	{
        var cx = task.minX + (task.maxX - task.minX) * i / task.width;
        var x = 0, y = 0;
 
        for (iter = 0; x*x + y*y < escape && iter < max_iter; iter++) 
	{
		/* z^2*/
            	var temp = x*x - y*y + cx;
            	y = 2*x*y + cy;
            	x = temp;
		
			
		/* z^3
		var temp = x*x*x - 3*x*y*y + cx;
		y = 2*x*x*y - y*y*y + cy;
		x = temp;
		*/
			
		/* z^4
		var temp = x*x*x*x - 5*x*x*y*y + y*y*y*y + cx;
		y = 3*x*x*x*y - 4*x*y*y*y + cy;
		x = temp;
		*/
			
		/*e^(z)
		var temp = Math.pow(Math.E,x), cos=Math.cos(y), sin=Math.sin(y);
		
		x = temp*cos+cx;
		y = temp*sin+cy;
		*/
		
		/* z^2*e^(z^2)
		var stepx = x*x-y*y, stepy = 2*x*y, temp=Math.pow(Math.E,stepx), cos=Math.cos(stepy), sin=Math.sin(stepy);
		
		x = temp*(stepx*cos - stepy*sin) + cx;
		y = temp*(stepx*sin + stepy*cos) + cy;
		*/
	}			
		
        if (iter == max_iter) iter = -1;
		
        task.values.push(iter);
    }
	
    return task;
}
 
onmessage = function (task)
{
    var workerResult = computeRow(task.data);
    postMessage(workerResult);
}
