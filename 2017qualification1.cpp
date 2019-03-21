// Score:  562500
// scored using https://github.com/vitowalteranelli/Google-HashCode-Playground
// naive simple greedy technique - assign priority to each request - number of requests * latency without caching
// place the max priority request into the closest cache server if enough space
// repeat till priority queue empty

#include <iostream>
#include <vector>
#include <queue>
#include <map>
using namespace std;

// order by max weight
static bool cmp(pair<int, int> p1, pair<int, int> p2)
{
	return p1.first > p2.first;
}

int main()
{
	int num_videos, num_endpoints, num_requests, num_servers, server_cap;
	cin >> num_videos >> num_endpoints >> num_requests >> num_servers >> server_cap;

	vector<int> server_space_remaining = vector<int>(num_servers, server_cap);

	// input the video size in megabytes for each video
	vector<int> video_sizes(num_videos);
	for(int i = 0 ; i < num_videos ; i++)
		cin >> video_sizes[i];

	int K; // number of cache servers an endpoint is connected to
	vector<int> hub_latencies = vector<int>(num_endpoints); // latency of serving video request from central hub to endpoints
	vector<vector<pair<int, int>>> server_latencies; // keep track of latencies to every cache server from each endpoint
	for(int endpoint = 0 ; endpoint < num_endpoints ; endpoint++)
	{
		cin >> hub_latencies[endpoint] >> K;
		int sid, lat;
		vector<pair<int, int>> row;
		for(int i = 0 ; i < K ; i++)
		{
			cin >> sid >> lat;
			row.push_back({sid, lat}); // push back server id and latency
		}
		server_latencies.push_back(row);
	}
	// for each request, save the video needed, the endpoint demanding it and the number of requests
	int vid, endpoint_id, req_count;
	vector<pair<int, pair<int, int>>> requests;
	for(int i = 0 ; i < num_requests ; i++)
	{
		cin >> vid >> endpoint_id >> req_count;
		pair<int, pair<int, int>> r = {vid, {req_count, endpoint_id}};
		requests.push_back(r);
	}

	// for all requests, push into priority_queue - <num_Requests * hub_latency, video_id>
	// pick max item and try to put that video in the most suitable cache server
	// pick next item and try to put in most suitable cache server etc
	// stop when all cache servers full
	// simple greedy method
	priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(&cmp)> pq(&cmp);
	for(int i = 0 ; i < requests.size() ; i++)
	{
		int hublat = hub_latencies[requests[i].second.second];
		pq.push({requests[i].second.first * hublat, requests[i].first});
	}

	map<int, vector<int>> results;
	while (!pq.empty())
	{
		auto ele = pq.top();
		pq.pop();
		if (ele.first == 0) continue; // videos with no requests can stay in hub

		// get the number of requests and endpoint id for this video
		int vid = ele.second;
		int numreq = -1, endp_id = -1, hublat = -1;
		for(int i = 0 ; i < requests.size() ; i++)
		{
			if (requests[i].first == vid)
			{
				numreq = requests[i].second.first;
				endp_id = requests[i].second.second;
				hublat = hub_latencies[requests[i].second.second];
				break;
			}
		}
		if (numreq == -1 || endp_id == -1) continue;

		// get the cache server with minimum latency
		int minlat = 1000000, min_server_id = -1;
		for(int i = 0 ; i < server_latencies[endp_id].size() ; i++)
		{
			if (server_latencies[endp_id][i].second < minlat)
			{
				minlat = server_latencies[endp_id][i].second;
				min_server_id = server_latencies[endp_id][i].first;
			}
		}

		// put the video in above cache server if space remaining, update cache space remaining
		if (server_space_remaining[min_server_id] >= video_sizes[vid])
		{
			server_space_remaining[min_server_id] -= video_sizes[vid];
			
			if (results.find(min_server_id) == results.end())
				results[min_server_id] = vector<int>();

			results[min_server_id].push_back(vid);
		}
	}

	cout << results.size() << endl;
	for(auto itr = results.begin() ; itr != results.end() ; itr++)
	{
		cout << itr->first << " ";
		for(int i = 0 ; i < itr->second.size() ; i++)
		{
			cout << itr->second[i] << " ";
		}
		cout << endl;
	}
}

