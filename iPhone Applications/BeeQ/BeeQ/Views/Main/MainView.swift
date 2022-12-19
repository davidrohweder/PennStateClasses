//
//  MainView.swift
//  BeeQ
//
//  Created by user224354 on 8/24/22.
//

import SwiftUI

enum Showing : String, Identifiable, CaseIterable {
    case hints, preferences
    var id: RawValue { rawValue }
}


struct MainView: View {
    @State private var showing: Showing?
    
    var body: some View {
        VStack {
            MainGameView()
            HStack (alignment: .bottom) {
                Button {
                    showing = .hints
                } label: {
                    Image(systemName: "info.circle.fill")
                }
                Button {
                    showing = .preferences
                } label: {
                    Image(systemName: "square.stack.3d.up")
                }
            }
        }
        .sheet(item: $showing) { item in
            switch item {
            case .hints: InfoView()
            case .preferences: PreferencesView()
            }
        }
        .accentColor(.black)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        MainView()
    }
}
