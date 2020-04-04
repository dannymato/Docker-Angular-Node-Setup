import { Injectable } from '@angular/core';
import { HttpClient } from '@angular/common/http'
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class GetDataService {

  constructor(private http: HttpClient) { }

  private _url = 'http://localhost:8000/test'

  public getData(): Observable<any> {
    return this.http.get(this._url, {
      params: {
        table: 'sample'
      }
    })
  }

}
