import {Component, Input, OnInit} from '@angular/core';
import {ActivatedRoute} from "@angular/router";


@Component({
  selector: 'app-display-register',
  templateUrl: './display-register.component.html',
  styleUrls: ['./display-register.component.css']
})
export class DisplayRegisterComponent implements OnInit {
  key: string;

  constructor(private route: ActivatedRoute) {
  }

  ngOnInit(): void {
    this.route.queryParams.subscribe(params => {
      this.key = params['key'];
      console.log(this.key)
    })
  }
}
