//---------------------------------------------------------------------
//
// Class: GraphDirected - Directed Graph
//
// This class implements a graph, where the
// following is true for all elements {u,v} in set E (set of edges):
//
//   o Edge {u,v} is ordered, i.e. {u,v} != {v,u}
//   o Edge {u,v} does not exist in E where u = v
//   o Edge {u,v} exists zero or one time in E
//

package bferris.graph;

import java.util.ArrayList;

public class GraphDirected extends GraphSimple {

	@Override
  public synchronized int      GetInDegree(Vertex vertex) {

  	int    count;
  	
  	if (vertex == null)
  		return 0;
  	count = 0;
  	for (Edge edge : edges) {
  		if (edge.v.Equals(vertex))
  			count++;
  	}
  	
  	return count;
  	
  }
  
	@Override
  public synchronized int      GetOutDegree(Vertex vertex) {

  	int    count;
  	
  	if (vertex == null)
  		return 0;
  	count = 0;
  	for (Edge edge : edges) {
  		if (edge.u.Equals(vertex))
  			count++;
  	}
  	
  	return count;
  	
  }
  
	@Override
  public synchronized int      CountPredecessors(Vertex vertex) {
    return GetInDegree(vertex);
  }
  
	@Override
  public synchronized int      CountSuccessors(Vertex vertex) {
  	return GetOutDegree(vertex);
  }
  
	@Override
  public synchronized Vertex[] GetPredecessors(Vertex vertex) {
  	
  	ArrayList<Vertex> list;
    Vertex[]          array;  	
  	
    do {
  		list = new ArrayList<Vertex>();
      if (vertices.size() == 0 || vertex == null)
      	break;
    	for (Edge edge : edges) {
    		if (edge.v.Equals(vertex))
    			list.add(edge.u);
    	}
    } while (false);
  	array = new Vertex[0];
  	
  	return list.toArray(array);
  	
  }
  
	@Override
  public synchronized Vertex[] GetSuccessors(Vertex vertex) {
  	
  	ArrayList<Vertex> list;
    Vertex[]          array;  	
  	
    do {
  		list = new ArrayList<Vertex>();
      if (vertices.size() == 0 || vertex == null)
      	break;
    	for (Edge edge : edges) {
    		if (edge.u.Equals(vertex))
    			list.add(edge.v);
    	}
    } while (false);
  	array = new Vertex[0];
  	
  	return list.toArray(array);
  	
  }

	@Override
	public synchronized Vertex[] GetSources() {

		ArrayList<Vertex> list;
    Vertex[]          array;  	
  	
 		list = new ArrayList<Vertex>();
   	for (Vertex v : vertices) {
   		if (GetInDegree(v) == 0)
   			list.add(v);
   	}
  	array = new Vertex[0];
  	
  	return list.toArray(array);  	
		
	}

	@Override
	public synchronized Vertex[] GetSinks() {

		ArrayList<Vertex> list;
    Vertex[]          array;  	
  	
    do {
  		list = new ArrayList<Vertex>();
    	for (Vertex v : vertices) {
    		if (GetOutDegree(v) == 0)
    			list.add(v);
    	}
    } while (false);
  	array = new Vertex[0];
  	
  	return list.toArray(array);  	
		
	}
	
	@Override
	public synchronized boolean IsCyclic() {

		CycleChecker checker;
		Vertex[] cycle;
		
		checker = new CycleChecker(this);
		cycle = checker.NextCycle();

		return (cycle != null);
		
	}
  
}
