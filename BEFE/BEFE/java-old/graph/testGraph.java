package bferris.graph;

import bferris.core.Id;
import java.io.PrintStream;
import java.util.ArrayList;
//import java.util.List;

public class testGraph {

		public static void testGraphSimple(String indent) {

			String      ind;
	    PrintStream out;
	    String[]    idstrings = {"{00010000-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010100-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010201-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010202-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020000-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020100-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020201-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020202-ffff-ffff-ffff-ffffffffffff}"
                         	    };
	    ArrayList<Id>     ids;
	    ArrayList<Vertex> vertices;
	    ArrayList<Edge>   edges;
	    Graph             graph;
	    int               i;
	    Vertex[]          adjacent;
	    
	    ind = indent + "  ";
	    out = System.out;
	    
	    out.println(indent+"GraphSimple...");

	    // Build the Ids
	    out.println(ind+"Building Ids...");
	    ids = new ArrayList<Id>();
	    //for (String idstring : idstrings)
	    //	ids.add(new Id(idstring));
	    for (i = 0; i < idstrings.length; i++)
      	ids.add(new Id());

	    // Build the Vertices
	    out.println(ind+"Building Vertices...");
	    vertices = new ArrayList<Vertex>();
	    for (Id id : ids)
      	vertices.add(new Vertex(id));
	    
	    // Build the Edges
	    out.println(ind+"Building Edges...");
	    edges = new ArrayList<Edge>();
      edges.add(new Edge(vertices.get(0),vertices.get(1)));	    
      edges.add(new Edge(vertices.get(0),vertices.get(2)));	    
      edges.add(new Edge(vertices.get(0),vertices.get(3)));	    
      edges.add(new Edge(vertices.get(4),vertices.get(5)));	    
      edges.add(new Edge(vertices.get(4),vertices.get(6)));	    
      edges.add(new Edge(vertices.get(4),vertices.get(7)));	    
	    
      // Build the GraphSimple...
	    out.println(ind+"Building Graph...");
      graph = new GraphSimple();
      for (Vertex v : vertices)
      	graph.Add(v);
      for (Edge e : edges)
      	graph.Add(e);
      
      // Display stats
	    out.println(ind+"Graph info...");
      out.println(ind+"  GetOrder() = "+graph.GetOrder());      
      out.println(ind+"  GetSize()  = "+graph.GetSize());
	    
      out.println(ind+"  graph.Contains(Vertex)...");
      for (i=0; i<vertices.size(); i++)
      	if (!graph.ContainsVertex(vertices.get(i)))
      		out.println(ind+"    graph.Contains(vertices["+i+"]) = false");
      out.println(ind+"  graph.Contains(Edge)...");
      for (i=0; i<edges.size(); i++)
      	if (!graph.ContainsEdge(edges.get(i)))
      		out.println(ind+"    graph.Contains(edges["+i+"])    = false");

      out.println(ind+"  graph.CountAdjacent...");
      out.println(ind+"    vertices[0] = "+graph.CountAdjacent(vertices.get(0)));
  		out.println(ind+"    vertices[1] = "+graph.CountAdjacent(vertices.get(1)));
  		out.println(ind+"    vertices[4] = "+graph.CountAdjacent(vertices.get(4)));
  		out.println(ind+"    vertices[5] = "+graph.CountAdjacent(vertices.get(5)));
      
  		out.println(ind+"  graph.GetAdjacent...");
      adjacent = graph.GetAdjacent(vertices.get(0));
      out.println(ind+"    vertices[0] = "+vertices.get(0).GetId().ToShortString());
      for (i=0;i<adjacent.length;i++)
      	out.println(ind+"    adjacent["+i+"] = "+adjacent[i].GetId().ToShortString());
  		
		}

		public static void testGraphDirected(String indent) {

			String      ind;
	    PrintStream out;
	    String[]    idstrings = {"{00010000-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010100-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010200-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010201-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00010202-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020000-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020100-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020200-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020201-ffff-ffff-ffff-ffffffffffff}"
	    		                    ,"{00020202-ffff-ffff-ffff-ffffffffffff}"
                         	    };
	    ArrayList<Id>     ids;
	    ArrayList<Vertex> vertices;
	    ArrayList<Edge>   edges;
	    Graph             graph;
	    int               i;
	    Vertex[]          predecessors;
	    Vertex[]          successors;
	    Vertex[]          sources;
	    Vertex[]          sinks;
	    
	    ind = indent + "  ";
	    out = System.out;
	    
	    out.println(indent+"GraphDirected...");

	    // Build the Ids
	    out.println(ind+"Building Ids...");
	    ids = new ArrayList<Id>();
	    for (String idstring : idstrings)
	    	ids.add(new Id(idstring));

	    // Build the Vertices
	    out.println(ind+"Building Vertices...");
	    vertices = new ArrayList<Vertex>();
	    for (Id id : ids)
	    	vertices.add(new Vertex(id));
	    
	    // Build the Edges
	    out.println(ind+"Building Edges...");
	    edges = new ArrayList<Edge>();
      edges.add(new Edge(vertices.get(0),vertices.get(1)));	    
      edges.add(new Edge(vertices.get(0),vertices.get(2)));
      edges.add(new Edge(vertices.get(2),vertices.get(3)));
      edges.add(new Edge(vertices.get(2),vertices.get(4)));
      edges.add(new Edge(vertices.get(5),vertices.get(6)));	    
      edges.add(new Edge(vertices.get(5),vertices.get(7)));
      edges.add(new Edge(vertices.get(7),vertices.get(8)));
      edges.add(new Edge(vertices.get(7),vertices.get(9)));
	    
      // Build the GraphDirected...
	    out.println(ind+"Building Graph...");
      graph = new GraphDirected();
      for (Vertex v : vertices)
      	graph.Add(v);
      for (Edge e : edges)
      	graph.Add(e);
      
      // Display stats
	    out.println(ind+"Graph info...");
      out.println(ind+"  GetOrder() = "+graph.GetOrder());      
      out.println(ind+"  GetSize()  = "+graph.GetSize());	    
      for (i=0;i<vertices.size();i++)
      	out.println(ind+"  Degree("+i+")  = "+graph.GetInDegree(vertices.get(i))+","+graph.GetOutDegree(vertices.get(i)));
      out.println(ind+"  graph.CountPredecessors...");
      out.println(ind+"    Count[0] = "+graph.CountPredecessors(vertices.get(0)));
  		out.println(ind+"    Count[1] = "+graph.CountPredecessors(vertices.get(1)));
  		out.println(ind+"    Count[2] = "+graph.CountPredecessors(vertices.get(2)));
  		out.println(ind+"    Count[3] = "+graph.CountPredecessors(vertices.get(3)));
  		out.println(ind+"    Count[4] = "+graph.CountPredecessors(vertices.get(4)));
      
  		out.println(ind+"  graph.GetPredecessors...");
      predecessors = graph.GetPredecessors(vertices.get(1));
      out.println(ind+"    vertices[1]   = "+vertices.get(1).GetId().toString().substring(0,10)+"...");
      for (i=0;i<predecessors.length;i++)
      	out.println(ind+"    predecessors["+i+"] = "+predecessors[i].GetId().toString().substring(0,10)+"...");
  		
      out.println(ind+"  graph.CountSuccessors...");
      out.println(ind+"    Count[0] = "+graph.CountSuccessors(vertices.get(0)));
  		out.println(ind+"    Count[1] = "+graph.CountSuccessors(vertices.get(1)));
  		out.println(ind+"    Count[2] = "+graph.CountSuccessors(vertices.get(2)));
  		out.println(ind+"    Count[3] = "+graph.CountSuccessors(vertices.get(3)));
  		out.println(ind+"    Count[4] = "+graph.CountSuccessors(vertices.get(4)));
      
  		out.println(ind+"  graph.GetSuccessors...");
  		successors = graph.GetSuccessors(vertices.get(0));
      out.println(ind+"    vertices[0]   = "+vertices.get(0).GetId().ToShortString());
      for (i=0;i<successors.length;i++)
      	out.println(ind+"    successors["+i+"] = "+successors[i].GetId().ToShortString());

  		out.println(ind+"  graph.GetSources...");
  		sources = graph.GetSources();
      for (i=0;i<sources.length;i++)
      	out.println(ind+"    sources["+i+"] = "+sources[i].GetId().ToShortString());

      out.println(ind+"  graph.GetSinks...");
  		sinks = graph.GetSinks();
      for (i=0;i<sinks.length;i++)
      	out.println(ind+"    sinks["+i+"]   = "+sinks[i].GetId().ToShortString());

      out.println(ind+"  IsCyclic = "+graph.IsCyclic());      

		}

		//-----------------------------------------------------------------
		//
		// Method: testCycleChecker - Test CycleChecker class
		//
		// What we do here is to create the following graph...
		//
		//           +------+------+
		//           |      |      |
		//           v      |      |
		//   {0} -> {2} -> {3} -> {4}
		//           ^      ^
		//           |      |
		//   {1} ----+------+
		//
		//   {5} -> {6} -> {7} -> {8} -> {9}
		//    ^                           |
		//    |                           |
		//    +---------------------------+
		//
		// This should give us the following cycles...
		//
		//   {2} -> {3} -> {2}
		//   {2} -> {3} -> {4} -> {2}
		//   {5} -> {6} -> {7} -> {8} -> {9} -> {5}
		//   
		
		public static void testCycleChecker(String indent) {

			String            ind;
	    PrintStream       out;
	    Vertex[]          vertices;
	    ArrayList<Edge>   edges;
	    Graph             graph;
	    int               i;
	    CycleChecker      checker;
	    Vertex[]          cycle;
	    
	    ind = indent + "  ";
	    out = System.out;
	    
	    out.println(indent+"CycleChecker...");

	    // Build the Vertices
	    out.println(ind+"Building Vertices...");
	    vertices = new Vertex[10];
	    for (i=0; i < 10; i++)
	    	vertices[i] = new Vertex();
	    
	    // Build the Edges
	    out.println(ind+"Building Edges...");
	    edges = new ArrayList<Edge>();

	    edges.add(new Edge(vertices[0],vertices[1]));	    
      edges.add(new Edge(vertices[1],vertices[2]));	    
      edges.add(new Edge(vertices[2],vertices[3]));	    
      edges.add(new Edge(vertices[3],vertices[4]));	    
      edges.add(new Edge(vertices[3],vertices[2]));	    
      edges.add(new Edge(vertices[4],vertices[2]));	    
      edges.add(new Edge(vertices[1],vertices[3]));	    

      edges.add(new Edge(vertices[5],vertices[6]));	    
      edges.add(new Edge(vertices[6],vertices[7]));	    
      edges.add(new Edge(vertices[7],vertices[8]));	    
      edges.add(new Edge(vertices[8],vertices[9]));	    
      edges.add(new Edge(vertices[9],vertices[5]));	    

      // Build the GraphDirected...
	    out.println(ind+"Building Graph...");
      graph = new GraphDirected();
      for (Vertex v : vertices)
      	graph.Add(v);
      for (Edge e : edges)
      	graph.Add(e);
      
      // Display stats
	    out.println(ind+"Graph info...");
      out.println(ind+"  GetOrder() = "+graph.GetOrder());      
      out.println(ind+"  GetSize()  = "+graph.GetSize());	    
      
      // Calculate and display the cycles...
    	out.println(ind+"Cycles...");
      checker = new CycleChecker(graph);
      while (true) {
      	cycle = checker.NextCycle();
      	if (cycle == null || cycle.length <= 0)
      		break;
      	out.print(ind+"  ");
      	for (i=0;i<cycle.length;i++) {
      		if (i != 0)
      			out.print(" -> ");
      		out.print(cycle[i].GetId().ToShortString());
      	}
     		out.print(" -> ");
      	out.println(cycle[0].GetId().ToShortString());
      }

		}
		
	  //----------------------------------------------------------------------------
	  //
	  // Method: test - Test Graph classes
	  //
	  //----------------------------------------------------------------------------

		public static void test() {
			test("");
		}

		public static void test(String indent) {

	    PrintStream out;
	    String      ind;
	    
	    out = System.out;
	    ind = indent+"  ";
	    
	    out.println(indent+"***Testing Graph...");

	    testGraphSimple(ind);
	    testGraphDirected(ind);
	    testCycleChecker(ind);
	    
	  }

}
