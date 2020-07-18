import { Component, OnInit } from '@angular/core';
import { BackendService } from '../backend.service';
import {SearchResult} from "../_models/searchResult";

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.css']
})
export class SearchComponent implements OnInit {
  searchResults$: SearchResult[] = [];
  searchAttribute: string = '';
  searchQuery: string = '';
  isDepartment = false;


  constructor(private backend: BackendService) { }

  ngOnInit(): void {
  }

  onSubmit() {
    if (this.searchAttribute === 'departmentNumber') {
      this.isDepartment = true;
    } else {
      this.isDepartment = false;
    }

    this.backend.getSearchResults(this.searchAttribute, this.searchQuery).subscribe(result => {
      this.searchResults$ = result['results'].map(res => {
        try{
          const entry: SearchResult = {
            givenName: res[0],
            sn: res[1],
            username: res[2],
            sshPublicKey: res[3],
            homePostalAddress: res[4]
          };
          console.log(entry);
          return entry;
        }catch (e) {
          const entry = {};
          return entry;
        }
      });
    });
  }
}
