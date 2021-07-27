package bferris.graph;

public class Edge {
	
	public Vertex u;
	public Vertex v;

	public Edge(Vertex u, Vertex v) {
	  if (u != null)
	  	this.u = new Vertex(u);
	  else
	  	this.u = null;
	  if (v != null)
	  	this.v = new Vertex(v);
	  else
	  	this.v = null;
	}
	
	public Edge Copy() {
	  return new Edge(u,v);
	}
	
	public boolean Equals(Edge that) {
		return (     u != null                        &&
				         v != null                        &&
				    that.u != null                        &&
				    that.v != null                        &&
				    that.u.Equals(u) || that.u.Equals(v)) &&
				   (that.v.Equals(u) || that.v.Equals(v)) &&
				   !that.v.Equals(u);
	}
}
