//
//  ContentView.swift
//  Pentominoes
//
//  Created by user224354 on 9/14/22.
//

import SwiftUI

struct MainView: View {
    var body: some View {
        ZStack (alignment: .topLeading) {
            MainBoardView() // create box - get main board image
            AvailableBoardsView()
            MechanicButtonsView() // Reset & Solve
            AvailablePiecesView()
        }
        .background(.orange)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
    }
}
