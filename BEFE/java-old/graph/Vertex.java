package bferris.graph;

import bferris.core.Id;

public class Vertex {

	protected Id id;
	
	public Vertex() {
    id = new Id();
	}
	
	public Vertex(Id id) {
		if (id == null)
			this.id = null;
		else {
			this.id = new Id(id);
		}
	}

	public Vertex(Vertex vertex) {
	  if (vertex == null)
	  	this.id = null;
	  else {
	  	this.id = new Id(vertex.GetId());
	  }
	}
	
	public Vertex Copy() {
	  return new Vertex(this);
	}
	
	public Id GetId() {
	  return id;
	}

	public boolean Equals(Vertex that) {
	  return (that != null && that.GetId().Equals(id));	
	}
	
	@Override
	public String toString() {
		if (id == null)
			return "null";
		else
			return id.toString();
	}
}
