import { Component, OnInit } from '@angular/core';
import {BackendService} from '../../backend.service';
import {User} from '../../_models/user';

@Component({
  selector: 'app-login-display',
  templateUrl: './login-display.component.html',
  styleUrls: ['./login-display.component.css']
})
export class LoginDisplayComponent implements OnInit {
  // variable to hold the user object
  public user: User;

  // dependency injection of the backend service for communication
  constructor(private backend: BackendService) { }

  // on page creation
  ngOnInit(): void {
    // get the user object from the backend
    // among others, it holds the department number,
    // given name, home postal address and surname needed in the html file
    this.user = this.backend.userValue;
    console.log(this.user.token);
  }
}
