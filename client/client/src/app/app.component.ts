import { Component, ViewChild } from '@angular/core';
import { GetDataService } from './get-data.service';
import { MatTable } from '@angular/material/table';

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css']
})
export class AppComponent {
  title = 'client';

  public dataArray = [
    {
      id: 56,
      name: 'Danny'
    },
    {
      id: 5,
      name: 'Someone'
    }
  ]

  columnsToDisplay = ['id', 'name'];

  @ViewChild(MatTable) table: MatTable<any>;

  constructor (private getService: GetDataService) {
    this.getRequest();
  }

  getRequest() {
    this.getService.getData().subscribe((data) => {
      console.log(data);
      this.dataArray = data;
      this.table.renderRows()
    }
    );
  }

}
