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
            var tmp = x*x - y*y + cx;
            y = 2 * x * y + cy;
            x = tmp;
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
