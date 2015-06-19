function computeRow(task) 
{
	var iter = 0;
	var cy = task.i;
	var max_iter = task.max_iter;
	var escape = task.escape * task.escape;
	task.values = [];
	
	var jx = task.jx;
	var jy = task.jy;
	var fr = task.fr;
    
	switch (fr)
	{
		case 0: 
		for (var i = 0; i < task.width; i++)
		{
			var cx = task.minX + (task.maxX - task.minX) * i / task.width;
			var x = (fr == 1 ? cx:0), y = (fr == 1 ? cy:0);
 
			for (iter = 0; x*x + y*y < escape && iter < max_iter; iter++) { var temp = x*x - y*y + cx; y = 2*x*y + cy; x = temp; }
			
			if (iter == max_iter) iter = -1;
			task.values.push(iter);
		}
		break;
		
		case 1: 
		for (var i = 0; i < task.width; i++)
		{
			var cx = task.minX + (task.maxX - task.minX) * i / task.width;
			var x = (fr == 1 ? cx:0), y = (fr == 1 ? cy:0);
 
			for (iter = 0; x*x + y*y < escape && iter < max_iter; iter++) { var temp = x*x - y*y + jx; y = 2*x*y + jy; x = temp; }
			
			if (iter == max_iter) iter = -1;
			task.values.push(iter);
		}
		break;
		
		case 2: 
		for (var i = 0; i < task.width; i++)
		{
			var cx = task.minX + (task.maxX - task.minX) * i / task.width;
			var x = (fr == 1 ? cx:0), y = (fr == 1 ? cy:0);
 
			for (iter = 0; x*x + y*y < escape && iter < max_iter; iter++) 
			{ 
				var temp = Math.pow(Math.E,x), cos=Math.cos(y), sin=Math.sin(y); 
				x = temp*cos - cx; 
				y = temp*sin - cy;
			}
			
			if (iter == max_iter) iter = -1;
			task.values.push(iter);
		}
		break;
		
		default: 
		for (var i = 0; i < task.width; i++)
		{
			var cx = task.minX + (task.maxX - task.minX) * i / task.width;
			var x = (fr == 1 ? cx:0), y = (fr == 1 ? cy:0);
 
			for (iter = 0; x*x + y*y < escape && iter < max_iter; iter++) 
			{ 
				var stepx = x*x-y*y, stepy = 2*x*y, temp=Math.pow(Math.E,stepx), cos=Math.cos(stepy), sin=Math.sin(stepy);
				x = temp*(stepx*cos - stepy*sin) + cx;
				y = temp*(stepx*sin + stepy*cos) + cy;
			}
			
			if (iter == max_iter) iter = -1;
			task.values.push(iter);
		}
		break;
		
	}
	
	return task;
}
 
onmessage = function (task)
{
	var workerResult = computeRow(task.data);
	postMessage(workerResult);
}
