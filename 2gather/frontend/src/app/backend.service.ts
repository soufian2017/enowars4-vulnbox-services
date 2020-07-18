import { Injectable } from '@angular/core';
import {HttpClient} from '@angular/common/http';
import {BehaviorSubject, Observable} from "rxjs";
import {User} from "./_models/user";
import {Router} from "@angular/router";
import {map} from "rxjs/operators";

@Injectable({
  providedIn: 'root'
})
export class BackendService {
  baseUrl = 'http://localhost:7787';
  private userSubject: BehaviorSubject<User>;
  public user: Observable<User>;


  constructor(private http: HttpClient, private router: Router) {
    this.userSubject = new BehaviorSubject<User>(JSON.parse(localStorage.getItem('user')));
    this.user = this.userSubject.asObservable();
  }

  public get userValue(): User {
    return this.userSubject.value;
  }


  getSearchResults(searchAttribute, searchQuery) {
    const searchUrl = this.baseUrl + '/ang/search?attribute='+searchAttribute+'&query='+searchQuery;
    return this.http.get(searchUrl);
  }

  postLogin(username, password) {
    const formData = new FormData();
    formData.append('user', username);
    formData.append('pass', password);
    return this.http.post<User>(this.baseUrl + `/ang/login`, formData)
      .pipe(map(user => {
        // store user details and jwt token in local storage to keep user logged in between page refreshes
        localStorage.setItem('user', JSON.stringify(user));
        this.userSubject.next(user);
        return user;
      }));
  }

  postRegister(username, password,  firstname, surname, department, address, phone) {
    const formData = new FormData();
    formData.append('user', username);
    formData.append('pass', password);
    formData.append('firstname', firstname);
    formData.append('surname', surname);
    formData.append('department', department);
    formData.append('address', address);
    formData.append('phoneNumber', phone);
    return this.http.post(this.baseUrl + '/ang/register', formData);
  }
}
