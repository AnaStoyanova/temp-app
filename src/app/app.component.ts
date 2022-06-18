import { Component, OnInit } from '@angular/core';
import { Options, LabelType, CustomStepDefinition } from '@angular-slider/ngx-slider';
import { faRotate } from '@fortawesome/free-solid-svg-icons';
import { ArduinoService } from './arduino.service';
import { Subscription } from 'rxjs';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent implements OnInit {
  currentTemp = "20";
  rotateIcon = faRotate;

  minValue: number = 15;
  maxValue: number = 35;
  options: Options = {
    floor: -5,
    ceil: 50,
    translate: (value: number, label: LabelType): string => {
      switch (label) {
        case LabelType.Low:
          return '<b>Min temp:</b> ' + value + '°C';
        case LabelType.High:
          return '<b>Max temp:</b> ' + value + '°C';
        default:
          return  value + '°C';
      }
    }
  };

  constructor (private arduinoService : ArduinoService) {}
  ngOnInit(): void {
    this.arduinoService.getCurrentTemp().subscribe(temp => {
      this.currentTemp = <string> temp;
    })
  }


  refreshTemp() {
    this.arduinoService.getCurrentTemp();
  }

  submitValues() {
    this.arduinoService.submitValues(this.minValue, this.maxValue);
  }
}
