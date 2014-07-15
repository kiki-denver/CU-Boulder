import java.util.List;
import java.util.ArrayList;

public class GraphTester
{
	public static void main(String[] args)
	{
		FestivalOfGraphs f = new FestivalOfGraphs();
		f.init("graph3.txt");
		
		List<Integer> dfs = f.DepthFirstList(0);
		System.out.println(dfs.toString());
		List<Integer> bfs = f.BreadthFirstList(0);
		System.out.println(bfs.toString());
		Graph g = f.DepthFirstSpanningTree(0);
		g.printAdjacencyMatrix();
		Graph h = f.BreadthFirstSpanningTree(0);
		h.printAdjacencyMatrix();		
		
		// extra credit; comment out if not writing these functions
		// Graph i = f.PrimsAlgorithm(0);
		// i.printAdjacencyMatrix();
		// ArrayList<String> sp = f.DijkstrasShortestPath(0);
		// System.out.println(bfs.toString());
		
	}
}
