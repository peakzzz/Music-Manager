/*
 * music_manager.cpp
 *
 *  Created on: Mar 22, 2015
 *      Author: Preeti Krishnan
 *      Description: Music Manager is a bare bones music playlist manager. MM allows user to create and
 *      manage multiple playlists by reusing central store of songs
 */

#include<iostream>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
using namespace std;


class MusicList
{
      public:
      int currentId;                            // Stores current record of how many Artist-Song pairs present
      int playlistIndex;                        // Index of total playlist present

      map<int,string> Artist;                   // Hash Map of all Artists
      map<int,string> Song;                     // Hash Map of all Songs
      map<int,string> PlaylistMap;              // Hash Map of all Playlists and their names
      map<string,string> PlaylistSongMap;       // Hash Map of all song indexes with their key as name of the playlist
      MusicList();

      // Main menu
      void CreatePlaylist(string);              // Create Playlist giving the name as string
      void EditPlaylist(int);                   // Edit Playlist Menu
      void PrintSong(int);                      // Print song given song index
      void PrintPlaylist(int);                  // Print Playlist given playlist index
      void RetrieveList(string);                // Retrieves list given playlist name
      void PrintSongOrPlaylist(string);         // Prints song or playlist
      void Search(string);                      // Search through the entire file to see if any artist or song matches search term
      void SortFunction(string);                // Sort entire file based on Artist or Title
      void Sort(map<int,string>, string);       // Sort Implementation

      // Playlist Menu
      void DeleteSong(int);                     // Delete Song given playlist index
      void InsertSong(int);                     // Insert song given playlist inde
      void PrintPlaylistMenu(int);              // Print Playlist given playlist index

};

MusicList:: MusicList()
{
	currentId = 100;
	int count =0;
	playlistIndex = 0;
	string line;
	  ifstream myfile ("SongList.txt");			// Open the SongsList text file
	  if (myfile.is_open())
	  {
		  //cout << "Artist"<< "\t\t" <<"  Title" <<endl;
		   cout<<"MARKETO MUSIC"<<endl;
		   for(int i=0; i<currentId; i++)
		    {
		    	while ( getline (myfile,line) )						// Read file and enter line by line
		    	{
		    		int length, space, t;
		    		t = line.length();
		    		for(length=0, space = 0; length < t; length++)	// Loop to make sure that Artist and Song go in the right maps
		    		{
		    			if (line[length] == ' ')					// Check whether there is a space on the Artist's name or Song's name
		    			{
		    				++space;
		    				if (space == 2)							// Check the space between Artist and Song
		    					break;
		    			}
		    			else
		    				space = 0;
		    		}

		    		Artist[count] = line.substr(0,length-1);		// Assign value to Artist map
		    		while(line[length] == ' ')
		    		{
		    			length++;
		    		}
		    		Song[count] = line.substr(length);				// Assign value to Song map
		    		count++;
		    	}
		    }
	    myfile.close();												// close file

	  }

	  else cout << "Unable to open file";							// Check case for File opening

}

// Create Playlist giving the name as string
void MusicList::CreatePlaylist(string name)
{
     if (PlaylistSongMap.count(name) == 0)     // This checks if the playlist name has been repeated. if not, we continue execution of the function
     {
                           PlaylistMap[playlistIndex] = name;    // Assign the name to playlist map
                           PlaylistSongMap[name] = "";           // Initialize the Playlist Song Map with the name of the playlist
                           cout<<name<<" has been created with index "<<playlistIndex;
                           playlistIndex++;                      // Increment the playlist index
     }
     else
     {
                           cout<<"Playlist name present. Provide different name";
     }
}

void MusicList::EditPlaylist(int index)
{
     // Note: Launch Edit playlist menu from here and ask if you want to choose Delete song or Insert song or Print playlist
     // Based on that launch the three below
     int option;
     cout<<"Select option: 1. Delete Song 2. Insert song 3. Print Playlist"<<endl;
     cin>>option;

     if (!((option == 1) || (option == 2) || (option == 3)))
        cout<<"Wrong option! Please give the right option!"<<endl;

     if (option == 1)
        DeleteSong(index);

     if (option == 2)
        InsertSong(index);

     if (option == 3)
        PrintPlaylist(index);

}

// Prints song given the song index
void MusicList::PrintSong(int index)
{
     currentId=100;
     if (index > currentId)
        cout<<"Song Index greater than current present songs\n";
     else
     {
         cout<<Artist[index]<<"  "<<Song[index]<<endl;
     }
}

// Prints playlist given playlist index
void MusicList::PrintPlaylist(int index)
{
     string temp = PlaylistSongMap[PlaylistMap[index]];         //retrieving the name of the playlist and then contents of the playlist - contents are song IDs
     RetrieveList(temp);                                        // Retrieve the contents individually
}

void MusicList::RetrieveList(string text)
{
     cout<<"The contents of the Playlist are: "<<endl;
     stringstream ss(text);                                     // convert it to stringstream to access it easier
     while(ss)
     {
              string tmpId;
              int Id = 0;
              ss >> tmpId;                                      // stringstream to string
              if ( ! (istringstream(tmpId) >> Id) )             // converts to integer
                 continue;                                       // if conversion fails, assign 0
              cout<<Artist[Id]<<"    "<<Song[Id]<<endl;         // Print Artist and Song title for each
     }
}

// Given option print either contents of song or playlist. If search term is not provided then return saying wrong term
void MusicList::PrintSongOrPlaylist(string text)
{
     if (text == "song")
     {
              for (int i = 0; i < currentId; i++)
              {
                  cout<<Artist[i]<<"   "<<Song[i]<<endl;
              }
     }
     else
     if (text == "playlist")
     {
              for (int j=0; j < playlistIndex; j++)
              {
                  string temp = PlaylistSongMap[PlaylistMap[j]];     //retrieving the name of the playlist and then contents of the playlist - contents are song IDs
                  RetrieveList(temp);
              }

     }
     else
     {
         cout<<"Wrong syntax! Enter either song or playlist!"<<endl;
     }
}

//Given search term, find if it is there in either Artist or Song

void MusicList::Search(string searchText)
{
     for (int index = 0; index < currentId; index++)
     {
        int length = 0, songLength = 0, artistLength = 0, searchLength = 0;
    	string tmpArtist, tmpSong;
        tmpArtist = Artist[index];
        artistLength = tmpArtist.length();
        while(length < artistLength)
        {
        	tmpArtist[length] = tolower(tmpArtist[length]);
        	length++;
        }
        length = 0;

        tmpSong = Song[index];
        songLength = tmpSong.length();
        while(length < songLength)
        {
                	tmpSong[length] = tolower(tmpSong[length]);
                	length++;
        }
        length = 0;

        searchLength = searchText.length();
        while(length < searchLength)
        {
                        	searchText[length] = tolower(searchText[length]);
                        	length++;
        }
    	 if (tmpArtist.find(searchText) != std::string::npos)    // Check if search text is present in Artist
         {
            cout<<Artist[index]<<"  "<<Song[index]<<endl;
         }
         else
         if (tmpSong.find(searchText) != std::string::npos)      // Check if search text is present in Title
         {
            cout<<Artist[index]<<"  "<<Song[index]<<endl;
         }
     }
}

// Interface for Sort Function based on Artist or Title
void MusicList::SortFunction(string checkString)
{
     if (checkString == "Title")
     {
    	 	 	 	 Sort(Song, checkString);
     }

     if (checkString == "Artist")
     {
                     Sort(Artist, checkString);
     }
}

// Implementation for Sort function
void MusicList::Sort(map<int,string> m1, string checkString)
{
     map<string, int> sortMap;                                                           // Reversed map to use the fact that key is sorted automatically in C++
     string temp;
     for (map<int, string>::iterator i = m1.begin(); i != m1.end(); ++i)                 // Check map and go ahead with sort
     {
    	 temp = i->second;
         if (sortMap.count(i->second) > 0)                                               //  Check if key has already been used. In this case, for same Artist names or Song names
         {
             while(1)
             {
                     temp.append("*");                                                   // Insert a astrisk in case of similar keys(same Artists or songs)
                     if (sortMap.count(temp) == 0)                                       // Check if there is more similar keys
                     {
                                             sortMap[temp] = i->first;                   // insert this in the final reversed map
                                             break;
                     }

             }
         }
         else
         {
             sortMap[i->second] = i->first;                                              // if that key is not present, go ahead and insert it into the reversed map
             //cout<<sortMap[i->second];
         }
     }
     for (map<string,int>::iterator i1 = sortMap.begin(); i1 != sortMap.end(); ++i1)     // Iterate through the reversed map and print
     {
    	 temp = i1->first;
         temp.erase(remove(temp.begin(), temp.end(),'*'), temp.end());             // Remove the asterisks from the reversed map

         if (checkString == "Title")
            cout<<temp<<"   "<<Artist[i1->second]<<endl;                                       // Print song and the Artist based on the index from reversed map
         else
            cout<<temp<<"   "<<Song[i1->second]<<endl;                                         // Print Artist and the song based on the index from reversed map
     }
}

void MusicList::DeleteSong(int index)
{
     int songIndex;
     cout<<"Please provide the Song ID to delete from the Playlist "<<index<<endl;
     cin>>songIndex;
     string updatedList;
     string playlistSongs = PlaylistSongMap[PlaylistMap[index]];                         // retrieve the playlist song Info
     stringstream ss(playlistSongs);                                                     //convert it to stringstream to access it easier
     while(ss)
     {
              string tmpId;
              int Id = 0;
              ss >> tmpId;
              if ( ! (istringstream(tmpId) >> Id) )
            	  continue;
              if (Id == songIndex)                                                        // In case this is the index to delete we will continue so that this is not used
                 continue;
              else
              {
                  updatedList.append(tmpId);                                             // Append the rest of the songs present
                  updatedList.append(" ");                                               // Introduce a space so that stringstream will work
              }

     }
     PlaylistSongMap[PlaylistMap[index]] = updatedList;

}

void MusicList::InsertSong(int index)
{
     string songIndex;
     cout<<"Please provide the Song ID to insert to the Playlist "<<index<<endl;
     cin>>songIndex;
     string playlistSongs = PlaylistSongMap[PlaylistMap[index]];

     if (playlistSongs.find(songIndex) == std::string::npos)                             // Check to see if this song is used before
     {
                  playlistSongs.append(songIndex);                                       // Append song id at the end of the list
                  playlistSongs.append(" ");                                             // Introduce space to make stringstream work
                  cout<<playlistSongs;
                  PlaylistSongMap[PlaylistMap[index]] = playlistSongs;                   // Saves the updated value of the hashmap with current song info
     }
     else
     {
                  cout<<"Song already present!!"<<endl;
     }
}

void MusicList::PrintPlaylistMenu(int index)
{
     PrintPlaylist(index);                                                                // Same as PrintPlaylist
}


int main()
{
	MusicList m;
	string song;
	int choice, n;
	char ans ='Y';

	while(toupper(ans) == 'Y')
	{
	cout<< endl<<"Main Menu: Enter options from 1 to 10"<<endl							//Switch case for options select
			<<"1:  Print song"<< endl
			<<"2:  Search song"<<endl
			<<"3:  Print playlist"<<endl
			<<"4:  Create playlist"<<endl
			<<"5:  Sort song"<<endl
			<<"6:  Insert song"<<endl
			<<"7:  Delete song"<<endl
			<<"8:  Edit playlist"<<endl
			<<"9:  PrintPlaylistMenu"<<endl
			<<"10: Quit"<<endl;

			cin>>choice;

			switch(choice)
			{
			case 1: cout<<"Enter the SongId to be printed: ";
			cin>>n;
				m.PrintSong(n);
				break;
			case 2:
				cout<<"Enter a Song to be searched: ";
				cin>> song;
				m.Search(song);
				break;
			case 3:
				cout<<"Enter a Playlist index to be printed: ";
				cin>> n;
				m.PrintPlaylist(n);
				break;
			case 4:
				cout<<"Enter a Playlist to be created: ";
				cin>> song;
				m.CreatePlaylist(song);
				break;
			case 5:
				cout<<"Sort Playlist by (Artist/Title): ";
				cin>> song;
				m.SortFunction(song);
				break;
			case 6:
				cout<<"Enter Playlist index to insert: ";
				cin>> n;
				m.InsertSong(n);
				break;
			case 7:
				cout<<"Enter Playlist index to delete: ";
				cin>> n;
				m.DeleteSong(n);
				break;
			case 8:
				cout<<"Edit Playlist index: ";
				cin>> n;
				m.EditPlaylist(n);
				break;
			case 9:
				cout<<"Print Playlist Menu: ";
				cin>> n;
				m.PrintPlaylistMenu(n);
				break;
			case 10:
				cout<<"Quit";
				exit(0);
			default:
				cout<<"Invalid choice"<<endl;
				break;
			}

			cout<<endl<<"Continue(y/n)? ";
			cin>>ans;
			cout<<endl;

	}
return 0;
}
