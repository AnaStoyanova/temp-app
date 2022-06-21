import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class ArduinoService {

  constructor(private http: HttpClient) { }
  arduinoUrl = 'http://192.168.1.8:80';

  getCurrentTemp(): Observable<string> {
    let headers = new HttpHeaders({ 'Content-Type': 'application/json' });
    let options = { headers: headers, crossDomain: true};
    return this.http.get<string>(this.arduinoUrl + '/curr-temp', options);
  }

  submitValues(minValue: number, maxValue: number) {
    let body = { minValue, maxValue };
    let headers = new HttpHeaders({'Content-Type': 'text/plain'});
    this.http.post(this.arduinoUrl + '/set-values', JSON.stringify(body), { headers: headers}).subscribe(data => {
      console.log(data);
    });
  }
}
