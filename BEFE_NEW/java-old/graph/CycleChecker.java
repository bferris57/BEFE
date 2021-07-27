package bferris.graph;

import java.util.ArrayList;
import java.util.Stack;

public class CycleChecker {
	
	private class StackEntry {
	  Vertex   vertex;       // Current vertex
	  Vertex[] successors;   // List of successors by int Id
	  int      successoridx; // Current successor index (into successors) 
	}
	
	Graph               graph;
  Stack<StackEntry>   stack;
  ArrayList<Vertex[]> cycles;
  ArrayList<Vertex>   toprocess;
  
	public CycleChecker(Graph g) {

		this.graph = g;
		toprocess = new ArrayList<Vertex>();

		// Populate vertices to process
		if (g != null) {
  		for (Vertex v : g.GetVertices())
  			toprocess.add(v);
		}
			
		// Populate initial stack
		stack = new Stack<StackEntry>();

		// Clear initial cycle list
		cycles = new ArrayList<Vertex[]>();
	}
	
	protected void Processed(Vertex v) {
	
	  for (int i=0; i<toprocess.size(); i++) {
	  	if (toprocess.get(i).Equals(v)) {
	  		toprocess.remove(i);
	  		break;
	  	}
	  }
	  return;
	}
	
	Vertex[] NormaliseCycle(Vertex[] cycle) {
		
		int      minid = -1;
		int      minidx = 0;
		int      i;
		Vertex[] result;
		
		if (cycle == null)
			return cycle;
    for (i=0;i<cycle.length;i++) {
    	if (i == 0 || cycle[i].GetId().ToInt() < minid) {
    		minid = cycle[i].GetId().ToInt();
    		minidx = i;
    	}
    }
    if (i == 0)
    	result = cycle;
    else {
      result = new Vertex[cycle.length];
      for (i=0;i<cycle.length;i++)
      	result[i] = cycle[(i+minidx)%cycle.length];
    }
    return result;
	}
	
	boolean RecordCycle(Vertex[] vertices) {
		int      len;
		int      idx;
		int      i;
		Vertex[] cycle;
		
		len = cycles.size();
		vertices = NormaliseCycle(vertices);
		// For each recorded cycle...
		for (idx=0; idx<len; idx++) {
			// Get the cycle
			cycle = cycles.get(idx);
			// Skip if not same cycle
			if (cycle.length != vertices.length)
				continue;
			for (i=0;i<cycle.length;i++) {
				if (!cycle[i].equals(vertices[i]))
					break;
			}
			if (i >= cycle.length)
				continue;
      // Found match
      return true;
		}
		// Not found, record it
		cycles.add(vertices);
	  return false;	
	}
	
	public synchronized Vertex[] NextCycle() {

    StackEntry entry;
    StackEntry newentry;
    int        stackidx;
    Vertex[]   cycle;
    int        i;
    
    cycle = null;
    
    while (toprocess.size() != 0) {
    
    	// If stack is empty, populate with first unprocessed one...
    	if (stack.empty()) {
    		if (toprocess.size() == 0)
    			break;
    		newentry = new StackEntry();
    		newentry.vertex = toprocess.get(0);
    		toprocess.remove(0);
    		newentry.successoridx = 0;
    		newentry.successors = graph.GetSuccessors(newentry.vertex);
    		stack.push(newentry);
    	}
    	
    	// Get top stack entry
    	entry = stack.peek();
    	Processed(entry.vertex);
    	// Finished with this entry if at end of successor list
    	if (entry.successoridx >= entry.successors.length) {
    		stack.pop();
    		continue;
    	}
    	
    	// Get the vertex
    	newentry = new StackEntry();
    	newentry.vertex       = entry.successors[entry.successoridx++];
    	newentry.successoridx = 0;
    	newentry.successors   = graph.GetSuccessors(newentry.vertex);
    	
    	// See if vertex already on stack...
    	for (stackidx=0; stackidx < stack.size(); stackidx++) {
    		if (stack.get(stackidx).vertex.Equals(newentry.vertex))
    			break;
    	}
    	
    	// If already on stack, we have a cycle...
    	if (stackidx < stack.size()) {
    	  
    		// Form and the cycle
    		cycle = new Vertex[stack.size()-stackidx];
    		for (i=0; i < cycle.length; i++)
    			cycle[i] = stack.get(stackidx+i).vertex;
    		
    		// If not already on cycle list, we found one...
    		if (!RecordCycle(cycle))
          break;
    		
    		// Already on cycle list, discard it
    		cycle = null;
    		
    		continue;
    	}
    	
    	// Not on stack, push it
    	if (newentry.successors.length > 0)
      	stack.push(newentry);
    	
    }
    
    // Return the cycle we found (if any)
		return cycle;
		
	}

}
