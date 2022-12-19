//
//  MainGameView.swift
//  BeeQ
//
//  Created by user224354 on 8/25/22.
//

import SwiftUI

struct MainGameView: View {
    var body: some View {
        ZStack {
            MainBackGroundView()
            VStack {
                AppHeaderView()
                SessionScoreView().padding(.top, 20)
                FoundWordsView()
                TypedCharacterView().padding()
                CharacterOptionView()
                MechanicOptionView()
            }
        }
    }
}

struct SwiftUIView_Previews: PreviewProvider {
    static var previews: some View {
        MainGameView()
    }
}
