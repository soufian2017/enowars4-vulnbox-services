import { async, ComponentFixture, TestBed } from '@angular/core/testing';

import { DisplayRegisterComponent } from './display-register.component';

describe('DisplayRegisterComponent', () => {
  let component: DisplayRegisterComponent;
  let fixture: ComponentFixture<DisplayRegisterComponent>;

  beforeEach(async(() => {
    TestBed.configureTestingModule({
      declarations: [ DisplayRegisterComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(DisplayRegisterComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
