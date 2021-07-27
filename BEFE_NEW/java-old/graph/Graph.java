//---------------------------------------------------------------------
//
// Interface: Graph - A mathematical Graph
//

package bferris.graph;

public interface Graph {

	// Graph Membership query
	public boolean  ContainsVertex(Vertex v);
  public boolean  ContainsEdge(Edge e);
  
  // Graph Membership lists
	public int      CountVertices();
	public Vertex[] GetVertices();
	public int      CountEdges();
	public Edge[]   GetEdges();
	public int      FindVertex(Vertex v);
	public int      FindEdge(Edge e);
	public Vertex   GetVertex(int idx);
	public Edge     GetEdge(int idx);
	
  // Graph Membership maintenance
  public Vertex   Add(Vertex v);
  public void     Remove(Vertex v);
  public Edge     Add(Edge edge);  
  public void     Remove(Edge edge);

  // Graph query
  public int      GetOrder();
  public int      GetSize();
  public int      CountAdjacent(Vertex v);
  public Vertex[] GetAdjacent(Vertex v);

  // Directed graph query
  public int      GetInDegree(Vertex v);
  public int      GetOutDegree(Vertex v);
  public int      CountPredecessors(Vertex v);
  public int      CountSuccessors(Vertex v);
  public Vertex[] GetPredecessors(Vertex v);
  public Vertex[] GetSuccessors(Vertex v);
  public Vertex[] GetSources();
  public Vertex[] GetSinks();
  public boolean  IsCyclic();
  
}
