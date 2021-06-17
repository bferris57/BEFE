//---------------------------------------------------------------------
//
// Class: GraphSimple - Undirected simple Graph
//
// This class implements an undirected simple graph, where vertices V
// are distinctly identified by an Id (16 bytes), and where the
// following is true for all elements {u,v} in set E (set of edges):
//
//   o Edge {u,v} is unordered, i.e. {u,v} = {v,u}
//   o Edge {u,v} does not exist in E where u = v
//   o Edge {u,v} exists zero or one time in E
//
// The distinctness of Id in the set V (set of vertices) is implied
// and enforced.
//

package bferris.graph;

import java.util.ArrayList;
import java.util.List;

public class GraphSimple implements Graph {
	
	protected List<Vertex> vertices; // Set V: Vertices (with distinct Id)
	protected List<Edge>   edges;    // Set E: Edges {u,v}
	
  public GraphSimple() {
  
  	vertices = new ArrayList<Vertex>();
  	edges    = new ArrayList<Edge>();
  	
  }

  //-------------------------------------------------------------------
  //
  // Graph Membership Query
  //
  //-------------------------------------------------------------------

  public synchronized boolean ContainsVertex(Vertex vertex) {
  	if (vertex != null) {
			for (Vertex v : vertices) {
				if (v.Equals(vertex))
	  			return true;
			}
  	}
  	return false;
  }

  public synchronized boolean ContainsVertex(int idint) {
		for (Vertex v : vertices) {
			if (v.GetId().ToInt() == idint)
  			return true;
		}
  	return false;
  }
  
  public synchronized boolean ContainsEdge(Edge edge) {

  	if (edge == null || edge.u == null || edge.v == null)
  		return false;
  	if (edge.u.equals(edge.v))
  		return false;
  	for (Edge curedge : edges) {
  		if ( curedge.Equals(edge))
  			return true;
  	}
  	return false;
  }

  //-------------------------------------------------------------------
  //
  // Graph Membership Lists
  //
  //-------------------------------------------------------------------
	
  public int CountVertices() {
  	return vertices.size();
  }
  
  public synchronized Vertex[] GetVertices() {

  	int i;
  	
  	Vertex[] vcopy = new Vertex[vertices.size()];
  	for (i=0;i<vertices.size();i++)
  		vcopy[i] = vertices.get(i);
  	return vcopy;

  }
  
  public synchronized Edge[] GetEdges() {

  	int i;
  	
  	Edge[] ecopy = new Edge[edges.size()];
  	for (i=0;i<edges.size();i++)
  		ecopy[i] = edges.get(i);
  	return ecopy;

  }
  
  public int CountEdges() {
  	return edges.size();
  }

	public int FindVertex(Vertex v) {
		int idx;
		for (idx=0;idx<vertices.size();idx++) {
			if (vertices.get(idx).Equals(v))
				break;
		}
		if (idx >= vertices.size())
			idx = -1;
		return idx;
	}

	public int      FindEdge(Edge e) {
  	int idx;
  	for (idx=0;idx<edges.size();idx++) {
	  	if (edges.get(idx).Equals(e))
		  	break;
  	}
  	if (idx >= vertices.size())
  		idx = -1;
  	return idx;
  }
	
	public Vertex   GetVertex(int idx) {
		
	  if (idx >= 0 && idx < vertices.size())
	  	return vertices.get(idx).Copy();
	  return null;

	}
	
	public Edge     GetEdge(int idx) {
		
	  if (idx >= 0 && idx < edges.size())
	  	return edges.get(idx).Copy();
	  return null;
		
	}
	
  //-------------------------------------------------------------------
  //
  // Graph Membership Maintenance
  //
  //-------------------------------------------------------------------
  
  public synchronized Vertex Add(Vertex vertex) {
  	Vertex added = null;
  	if (vertex != null) {
      if (!ContainsVertex(vertex)) {
    		added = new Vertex(vertex);
      	vertices.add(added);
      }
  	}
    return added;
  }

  public synchronized void Remove(Vertex vertex) {

  	int    i;
  	
  	if (vertex != null) {
  		for (i=0;i<vertices.size();i++) {
  			if (vertices.get(i).equals(vertex)) {
  				vertices.remove(i);
  				break;
  			}
  		}
    	for (i=edges.size()-1; i >= 0; i--)
    		if (edges.get(i).u.Equals(vertex) ||	edges.get(i).v.Equals(vertex))
    			edges.remove(i);
  	}
    return;
  }

  public synchronized Edge Add(Edge edge) {

  	Edge added = null;
  	
  	if (edge != null && edge.u != null && edge.v != null && !ContainsEdge(edge)) {
  		if (!edge.u.Equals(edge.v)) {
  			added = new Edge(edge.u,edge.v);
        edges.add(added);
  		}
    }
  	
    return added;
  }
  
  public synchronized void Remove(Edge edge) {

    Edge curedge;
    int  i;
    
    do {
    	if (edge == null || edge.u == null || edge.v == null)
    		break;
	  	if (edge.u.equals(edge.v))
	  		break;
	  	for (i=edges.size() - 1; i >= 0; i-- ) {
	  		curedge = edges.get(i);
	  		if (edge.Equals(curedge)) {
	  			edges.remove(i);
  				break;
	  		}
	  	}		
    } while (false);  	
  	return;
  }
  
  //-------------------------------------------------------------------
  //
  // Graph query
  //
  //-------------------------------------------------------------------
  
  public synchronized int GetOrder() {

  	return vertices.size();
  	
  }

  public synchronized int GetSize() {

  	return edges.size();
  	
  }

  public synchronized int CountAdjacent(Vertex vertex) {

  	int    count;
  	
  	if (vertex == null)
  		return 0;
  	count = 0;
  	for (Edge edge : edges) {
  		if (edge.u.Equals(vertex) || edge.v.Equals(vertex))
  			count++;
  	}
  	
  	return count;
  	
  }

  public synchronized Vertex[] GetAdjacent(Vertex vertex) {
  	
  	ArrayList<Vertex> list;
    Vertex[]          adjacent;  	
  	do {
  		list = new ArrayList<Vertex>();
      if (vertices.size() == 0 || vertex == null)
      	break;
    	for (Edge edge : edges) {
    		if (edge.u.Equals(vertex))
    			list.add(new Vertex(edge.v));
    		else if (edge.v.equals(vertex))
    			list.add(new Vertex(edge.u));
    	}
    } while (false);
  	adjacent = new Vertex[0];
  	return list.toArray(adjacent);
  	
  }
  
  //-------------------------------------------------------------------
  //
  // Directed Graph Query
  //
  //-------------------------------------------------------------------
  
  public int      GetInDegree(Vertex v)       {return 0;}
  public int      GetOutDegree(Vertex v)      {return 0;}
  public int      CountPredecessors(Vertex v) {return 0;}
  public int      CountSuccessors(Vertex v)   {return 0;}
  public Vertex[] GetPredecessors(Vertex v)   {return new Vertex[0];}
  public Vertex[] GetSuccessors(Vertex v)     {return new Vertex[0];}
  public Vertex[] GetSources()                {return new Vertex[0];}
  public Vertex[] GetSinks()                  {return new Vertex[0];}
  public boolean  IsCyclic()                  {return false;}
}
