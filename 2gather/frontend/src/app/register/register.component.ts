import { Component, OnInit } from '@angular/core';
import { BackendService } from '../backend.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent implements OnInit {
  // variable to hold the private key value from the backend response
  public key: string;

  // variables for to way binding to hold the input values
  public user: string;
  public firstname: string;
  public surname: string;
  public department: string;
  public address: string;
  public pass: string;
  public phone: string;

  // dependency injection of the backend service for communication
  // and the angular specific router
  constructor(private backend: BackendService, private router: Router) { }

  ngOnInit(): void {
  }

  // call of the register function on click of the register button in the html file
  onRegister() {
      // default option in case floor is empty
      if (this.department === '') {
        this.department = 'floor1';
      }
      // default option in case the address is left empty
      if (this.address === '') {
        this.address = 'Nowhere';
      }
      // call the backend service, the form arguments are passed along
      this.backend.postRegister(
        this.user,
        this.pass,
        this.firstname,
        this.surname,
        this.department,
        this.address,
        this.phone
      ).subscribe(data => {
        // on valid response the private key is extracted
        this.key = data['pk'];
        // the user is redirected to the reister-display component
        // the private key is passed along as a query parameter to be displayed
        this.router.navigate(['/register-display'], {queryParams: {key: this.key}});
        console.log(this.key);
      });
  }
}
