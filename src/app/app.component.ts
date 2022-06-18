import { Component } from '@angular/core';
import { Options, LabelType, CustomStepDefinition } from '@angular-slider/ngx-slider';
import { faRotate } from '@fortawesome/free-solid-svg-icons';


@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
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

  refreshTemp() {
    console.log("hello");
  }
}
